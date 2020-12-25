/*
Definitions and constant buffers
*/
#include "polyflags.fxh"


struct PS_OUTPUT
{
	float4 color: SV_Target0;
};


struct PS_OUTPUT2
{
	float4 color: SV_Target0;
	#if(ASSAO_ENABLED==1  || SSR_ENABLED==1)
	float4 normal: SV_Target1;
	#endif
};

struct PS_OUTPUT3
{
	float4 color: SV_Target0;
	#if(ASSAO_ENABLED==1 || SSR_ENABLED==1)
	float4 normal: SV_Target1;
	#endif
	#if(SSR_ENABLED==1)
	float ssrWg: SV_Target2;
	#endif
};

float4 unrealColor(float4 color, uint flags)
{
	float4 result;
	if(flags&PF_Modulated) //Modulated not influenced by color
		result= float4(1,1,1,1);
	
	result = clamp(color,0,1); //Color is sometimes >1 for Rune which screws up runestone particles

	return result;
}

/**
Calculate fog color
*/
float4 unrealVertexFog(float4 fog, uint flags)
{
	float4 result;
	if((flags & (PF_RenderFog|PF_Translucent|PF_Modulated|PF_AlphaBlend))==PF_RenderFog)
		result = fog;
	else
		result = float4(0,0,0,0);
	return result;
}

/**
Increase dynamic range of a color for when HDR is enabled
*/
float3 increaseDynamicRange(float3 color,uint flags)
{
	float3 output=color;
	if(!(flags&PF_Modulated))
		output.rgb=pow(abs(output.rgb)*1.2f,1.2);
	return output;
}

/**
Handle diffuse texturing/alpha test

*/
float4 diffuseTexture(float4 diffuse, float4 diffusePoint, uint flags)
{
	//Alpha test; point sample to get rid of seams
	if(flags&PF_Masked && !(flags&(PF_Translucent|PF_AlphaBlend))) //Need to check alphablend here because external textures can have alpha channel
	{
		#if(!ALPHA_TO_COVERAGE_ENABLED)
		clip(diffusePoint.a-0.5f);
		clip(diffuse.a-0.5f);
		#endif
	}
	
	if(flags&PF_NoSmooth)
	{
		diffuse = diffusePoint;
	}
	#if (NO_FILTERING==1)
	return diffusePoint;
	#else
	return diffuse;
	#endif
}