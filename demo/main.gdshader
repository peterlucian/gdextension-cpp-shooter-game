shader_type spatial;

uniform vec4 grid_color = vec4(0.0, 0.0, 0.0, 1.0); // Color of the grid lines
uniform float grid_size = 20.0;  // Total grid size (e.g., 20 for a 20x20 grid)
uniform float cell_size = 1.0;   // Size of each grid cell
uniform float line_width = 0.05; // Width of the grid lines

void fragment() {
    // Scale UVs by the cell size to get grid cells
    vec2 uv = UV.xy * (grid_size / cell_size);

    // Calculate grid lines
    vec2 grid = abs(fract(uv) - 0.5) / fwidth(uv);
    float line = min(grid.x, grid.y);
    float grid_line = smoothstep(0.5 - line_width, 0.5 + line_width, line);

    // Apply the grid color to the floor
    ALBEDO = mix(ALBEDO, grid_color.rgb, 1.0 - grid_line);
}