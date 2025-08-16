/**
 * Generates an array of HSL colors distributed evenly around the color wheel
 * @param {number} count - Number of colors to generate
 * @param {number} startHue - Starting hue (default: 0 for red)
 * @returns {string[]} Array of HSL color strings
 */
export function generateColors(count, startHue = 0) {
    const colors = [];
    for (let i = 0; i < count; i++) {
        const hue = (startHue + (i * 360 / count)) % 360;
        const saturation = 80;
        const lightness = 50;
        colors.push(`hsl(${hue}, ${saturation}%, ${lightness}%)`);
    }
    return colors;
}

/**
 * Returns a contrast color for better text readability
 * @param {string} hslColor - The background HSL color
 * @returns {string} RGB color string for text
 */
export function getContrastColor(hslColor) {
    // For now, always return white. Could be enhanced to calculate actual contrast
    return `rgb(255, 255, 255)`;
}
