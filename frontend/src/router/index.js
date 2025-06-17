import { createRouter, createWebHistory } from "vue-router";
import HomeView from "../views/HomeView.vue";
import CausalGraphView from "@/views/CausalGraphView.vue";
import UploadFilesView from "@/views/UploadFilesView.vue";

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: "/",
      name: "home",
      component: HomeView,
    },
    {
      path: "/causal",
      name: "CausalGraph",
      component: CausalGraphView,
    },
    {
      path: "/transition",
      name: "DomainTransitionGraph",
      component: CausalGraphView,
    },
    {
      path: "/upload",
      name: "Upload",
      component: UploadFilesView,
    },
  ],
});

export default router;
