#pragma once
#include "DXUT.h"
#include "SDKmisc.h"
#include "Effects.h"



class RobotEffectFactory : public DirectX::EffectFactory
{
	private:
		WCHAR searchPath[MAX_PATH];

	public:
		RobotEffectFactory(_In_ ID3D11Device* device);
		virtual void __cdecl RobotEffectFactory::CreateTexture(_In_z_ const WCHAR* name, _In_opt_ ID3D11DeviceContext* deviceContext, _Outptr_ ID3D11ShaderResourceView** textureView) override;
		void SetPath(const WCHAR* path);

};

