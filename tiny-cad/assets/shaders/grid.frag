#version 460 core

in vec3 vWorldPos;
uniform float uCamToTargetDist;

out vec4 FragColor;

// color codes
const vec3 minorLineColor = vec3(0.24, 0.25, 0.28);
const vec3 majorLineColor = vec3(0.34, 0.35, 0.39);
const vec3 floorColor     = vec3(0.17, 0.18, 0.20);

const vec3 xAxisColor = vec3(0.85, 0.30, 0.30);   
const vec3 zAxisColor = vec3(0.30, 0.45, 0.90); 

// turns a distance in pixels into a 0..1 line amount..
// thickness is the line width in pixels..
float lineAmount(float pixels, float thickness) { return 1.0 - clamp(pixels / thickness, 0.0, 1.0); }

vec2 compensate(vec2 v) {

	vec2 dx = dFdx(v);
	vec2 dy = dFdy(v);
	return sqrt(dx * dx + dy * dy);
}

// 0..1: how much this pixel lies on a grid line of the given spacing.
float gridLine(vec2 pos, float spacing, float thickness) {
	
	vec2 coord  = pos / spacing;
	vec2 w      = compensate(coord);          
	vec2 f      = fract(coord);
	vec2 d      = min(f, 1.0 - f);
	vec2 pixels = d / w;

	float line = max(lineAmount(pixels.x, thickness), lineAmount(pixels.y, thickness));
	float density = max(w.x, w.y);
	float fade    = 1.0 - smoothstep(0.1, 0.3, density);

	return line * fade;
}

// 0..1: how much this pixel lies on the line where coord is 0.
float axisLine(float coord, float thickness) {

	float pixels = abs(coord) / fwidth(coord);
	return lineAmount(pixels, thickness);
}
 
void main() {
	
    // which current power of 10, and how far toward the next one (0..1)
	float lod =  log(uCamToTargetDist / 20.0) / log(10.0); 

	float level = max(floor(lod), 0.0);                
	float blend = (floor(lod) < 0.0) ? 0.0 : fract(lod);

	float fineSpacing   = pow(10.0, level); 
	float midSpacing    = fineSpacing * 10.0;
	float coarseSpacing = fineSpacing * 100.0;

	// fine lines calculations
	float fineFade   = 1.0 - smoothstep(0.6, 1.0, blend);
	float coarseFade = smoothstep(0.6, 1.0, blend);

	float fineAmount   = gridLine(vWorldPos.xz, fineSpacing,   1.0) * fineFade;
	float midAmount    = gridLine(vWorldPos.xz, midSpacing,    1.0);
	float coarseAmount = gridLine(vWorldPos.xz, coarseSpacing, 1.0) * coarseFade;

	vec3 midColor = mix(majorLineColor, minorLineColor, coarseFade);

	// Axis color markings
	float xAxisAmount = axisLine(vWorldPos.z, 0.5);   
	float zAxisAmount = axisLine(vWorldPos.x, 0.5);   

	// final color blending
    vec3 color = floorColor;
	color = mix(color, minorLineColor, fineAmount);
	color = mix(color, midColor,       midAmount);
	color = mix(color, majorLineColor, coarseAmount);
	color = mix(color, xAxisColor,     xAxisAmount);
	color = mix(color, zAxisColor,     zAxisAmount);

	FragColor = vec4(color, 1.0);
}