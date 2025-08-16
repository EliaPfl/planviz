/**
 * Checks if dark mode is currently active
 * @returns {boolean} True if dark mode is active
 */
export function isDarkMode() {
    return document.documentElement.classList.contains('dark') ||
        window.matchMedia('(prefers-color-scheme: dark)').matches;
}
