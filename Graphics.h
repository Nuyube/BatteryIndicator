#include<d2d1.h>
#include<Windows.h>
#include<WbemIdl.h>
#include<WbemCli.h>
#include<fstream>
#include<string>
#pragma comment(lib, "Wbemuuid.lib")

using namespace D2D1;

const BSTR wql = SysAllocString(L"WQL");
const BSTR fcc = SysAllocString(L"FullChargedCapacity");
const BSTR rc = SysAllocString(L"RemainingCapacity");
const BSTR sfcc = SysAllocString(L"Select * FROM BatteryFullChargedCapacity");
const BSTR sbs = SysAllocString(L"Select * from BatteryStatus");

class Graphics {
public:
	void reloadConfig() {
		std::fstream f;
		f.open("C:/ProgramData/Nuyube/BatteryIndicator/settings.txt", std::ios::in);
		while (!f.eof()) {
			auto updatePercentage = [](const char* token, std::string line, float& toMod) {
				if (line.find(token) != line.npos) {
					int percentI = line.find('%');
					if (line.find('%') == line.npos) return;
					percentI -= 2;
					char twochar[3];
					memcpy(&twochar[0], &line[percentI], 2);
					twochar[2] = '\0';
					int percentage = atoi(twochar);
					toMod = percentage;
				}
			};
			auto updateColor = [](const char* token, std::string line, ID2D1SolidColorBrush* toMod) {
				if (line.find(token) != line.npos) {
					int percentI = line.find("0x");
					if (line.find("0x") == line.npos) return;
					percentI += 2;
					char twochar[7];
					memcpy(&twochar[0], &line[percentI], 6);
					twochar[6] = '\0';
					uint32_t percentage;
					sscanf_s(twochar, "%x", &percentage);
					toMod->SetColor(ColorF(percentage));
				}
			};
			char lineholder[256];
			f.getline(lineholder, 256);
			std::string line = lineholder;
			//Disable comments.
			if (lineholder[0] == '#') { continue; }
			updatePercentage("LOW", line, m_lowPer);
			updatePercentage("WARN", line, m_warnPer);
			updatePercentage("CRIT", line, m_critPer);
			updateColor("LOW", line, m_lowBrush);
			updateColor("WARN", line, m_warnBrush);
			updateColor("CRIT", line, m_critBrush);
			updateColor("FULL", line, m_fullBrush);
			updateColor("CHARGE", line, m_chargeBrush);
			updateColor("EMPTY", line, m_emptyBrush);
		}
		f.close();
	}
	Graphics(HWND h, POINT size) {
		CoInitializeEx(0, COINIT_MULTITHREADED);
		CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&m_locator);

		HRESULT HR = m_locator->ConnectServer(BSTR(L"ROOT\\WMI"), 0, 0, 0, 0, 0, 0, &m_services);
		CoSetProxyBlanket(m_services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

		D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_factory);
		m_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(h, D2D1::SizeU(size.x, size.y)), &m_rendertarget);
		m_rendertarget->CreateSolidColorBrush(ColorF(0x6666FF), &m_chargeBrush);
		m_rendertarget->CreateSolidColorBrush(ColorF(0x00FF00), &m_fullBrush);
		m_rendertarget->CreateSolidColorBrush(ColorF(0xFF0088), &m_warnBrush);
		m_rendertarget->CreateSolidColorBrush(ColorF(0xFF00FF), &m_lowBrush);
		m_rendertarget->CreateSolidColorBrush(ColorF(0xFF0000), &m_critBrush);
		m_rendertarget->CreateSolidColorBrush(ColorF(0x000000), &m_emptyBrush);
	}
	~Graphics() {
		m_rendertarget->Release();
		m_services->Release();
		m_factory->Release();
		m_locator->Release();
		CoUninitialize();
		m_batPer = 0;
	}
	unsigned __int32 executeQuery(BSTR query, BSTR token, VARIANT* v) {
		IEnumWbemClassObject* enumer;
		IWbemClassObject* battery;
		ULONG rowsreturned;

		m_services->ExecQuery(wql, query, 0, 0, &enumer);

		if (enumer == nullptr) return 0;

		enumer->Next(0xF, 1, &battery, &rowsreturned);

		if (rowsreturned <= 0) return 0;

		battery->Get(token, 0, v, 0, 0);

		battery->Release();
		enumer->Release();
		ZeroMemory(battery, sizeof(IWbemClassObject));
		ZeroMemory(enumer, sizeof(IEnumWbemClassObject));
		return v->ulVal;
	}
	void update() {
		unsigned __int32 full, remaining;

		reloadConfig();
		SYSTEM_POWER_STATUS ps;
		GetSystemPowerStatus(&ps);
		VARIANT v;
		VariantInit(&v);

		full = executeQuery(sfcc, fcc, &v);

		remaining = executeQuery(sbs, rc, &v);

		VariantClear(&v);
		ZeroMemory(&v, sizeof(VARIANT));

		//Either of the values was 0.
		//It doesn't make sense for remaining to be 0 and the PC to still be on,
		//and it doesn't make sense for the full capacity to be 0.
		if (!remaining || !full)
			return;

		m_batPer = remaining * 100.0f / (float)full;

		draw(ps);

		ZeroMemory(&ps, sizeof(ps));
	}
	void draw(SYSTEM_POWER_STATUS ps) {
		m_rendertarget->BeginDraw();

		m_rendertarget->Clear(m_emptyBrush->GetColor());
		m_rendertarget->DrawLine(Point2F(0, 0.5), Point2F((float)m_rendertarget->GetPixelSize().width * (m_batPer / 100.), 0.5),
			(ps.ACLineStatus) ? m_chargeBrush :
			(m_batPer < m_critPer) ? m_critBrush :
			(m_batPer < m_warnPer) ? m_warnBrush :
			(m_batPer < m_lowPer) ? m_lowBrush :
			(m_batPer > m_lowPer) ? m_fullBrush : m_critBrush,
			m_rendertarget->GetPixelSize().height * 2);

		m_rendertarget->EndDraw();
	}
private:
	ID2D1SolidColorBrush* m_critBrush, * m_chargeBrush, * m_lowBrush, * m_warnBrush, * m_fullBrush, * m_emptyBrush;
	ID2D1Factory* m_factory;
	ID2D1HwndRenderTarget* m_rendertarget;
	IWbemLocator* m_locator;
	IWbemServices* m_services;
	float m_batPer;
	float m_lowPer = 25.0f;
	float m_warnPer = 15.0f;
	float m_critPer = 10.0f;
};