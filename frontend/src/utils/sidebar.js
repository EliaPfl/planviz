/**
 * Scrolls the right sidebar to the top with smooth animation
 */
export function scrollSidebarToTop() {
    const rightSidebar = document.getElementById('right');
    if (rightSidebar) {
        const scrollContainer = rightSidebar.querySelector('.overflow-y-auto');
        if (scrollContainer) {
            scrollContainer.scrollTo({ top: 0, behavior: 'smooth' });
        }
    }
}
