import { createRouter, createWebHistory } from "vue-router";
import CausalGraphView from "@/views/CausalGraphView.vue";
import DomainTransitionGraphView from "@/views/DomainTransitionGraphView.vue";
import UploadFilesView from "@/views/UploadFilesView.vue";
import LandmarkGraphView from "@/views/LandmarkGraphView.vue";

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: "/",
      redirect: '/causal',
    },
    {
      path: "/causal",
      name: "CausalGraph",
      component: CausalGraphView,
    },
    {
      path: "/transition/:ID",
      name: "DomainTransitionGraph",
      component: DomainTransitionGraphView,
    },
    {
      path: "/landmark",
      name: "LandmarkGraph",
      component: LandmarkGraphView,
    },
    {
      path: "/upload",
      name: "Upload",
      component: UploadFilesView,
    },
  ],
});

export default router;
