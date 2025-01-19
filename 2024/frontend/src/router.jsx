import { createBrowserRouter } from "react-router-dom";
import MapPage, {loader as loaderMapData} from "./pages/MapPage";

const router = createBrowserRouter([
    {
        path: '/',
        children: [
            {
                index: true,
                element: <MapPage/>,
                loader: loaderMapData,
            },
        ]
    }
])

export default router;