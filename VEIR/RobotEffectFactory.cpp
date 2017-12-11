#include "RobotEffectFactory.h"


RobotEffectFactory::RobotEffectFactory(_In_ ID3D11Device* device) : EffectFactory(device)
{
	*searchPath = 0;
}

void RobotEffectFactory::SetPath(const WCHAR * path)
{
}

void RobotEffectFactory::CreateTexture(const WCHAR * name, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView ** textureView)
{
	WCHAR fname[MAX_PATH] = { 0 };
	if (*searchPath)
		wcscpy_s(fname, searchPath);
	wcscat_s(fname, name);

	WCHAR path[MAX_PATH] = { 0 };
	if (FAILED(DXUTFindDXSDKMediaFileCch(path, MAX_PATH, fname)))
	{
		throw std::exception("Media not found");
	}

	EffectFactory::CreateTexture(path, deviceContext, textureView);
}

