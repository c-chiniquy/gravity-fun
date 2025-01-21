dxc.exe -T cs_6_6 -E CS_MassParticles -Fo "compiled/CS_MassParticles.cso" "hlsl/ComputeParticles.hlsl"
dxc.exe -T cs_6_6 -E CS_VisualParticles -Fo "compiled/CS_VisualParticles.cso" "hlsl/ComputeParticles.hlsl"
dxc.exe -T vs_6_6 -E VS_Rects -Fo "compiled/VS_Rects_UniColor.cso" -D UNICOLOR "hlsl/RenderPrimitives.hlsl"
dxc.exe -T vs_6_6 -E VS_Rects -Fo "compiled/VS_Rects_ColorBuffer.cso" -D COLORBUFFER "hlsl/RenderPrimitives.hlsl"
dxc.exe -T vs_6_6 -E VS_Points -Fo "compiled/VS_Points_UniColor.cso" -D UNICOLOR "hlsl/RenderPrimitives.hlsl"
dxc.exe -T vs_6_6 -E VS_Points -Fo "compiled/VS_Points_ColorBuffer.cso" -D COLORBUFFER "hlsl/RenderPrimitives.hlsl"
dxc.exe -T ps_6_6 -E PS_Basic -Fo "compiled/PS_Basic.cso" "hlsl/RenderPrimitives.hlsl"
dxc.exe -T vs_6_6 -E VS_Heatmap -Fo "compiled/VS_Heatmap.cso" "hlsl/RenderHeatmap.hlsl"
dxc.exe -T ps_6_6 -E PS_Heatmap -Fo "compiled/PS_Heatmap_Grayscale.cso" -D HEATMAP_GRAYSCALE "hlsl/RenderHeatmap.hlsl"
dxc.exe -T ps_6_6 -E PS_Heatmap -Fo "compiled/PS_Heatmap_Color.cso" -D HEATMAP_COLOR "hlsl/RenderHeatmap.hlsl"
dxc.exe -T cs_6_6 -E CS_Heatmap -Fo "compiled/CS_Heatmap.cso" "hlsl/ComputeHeatmap.hlsl"

pause
