import { useState } from "react"
import { getCoors } from "../js/api";

export default function Filters({setViewport}) {
    const [searcher, setSearcher] = useState('');

    return (
        <div className="absolute flex gap-5 top-0 left-0 w-full z-10 p-4">

            <div className="bg-white rounded-2xl py-2 px-4 flex gap-3 w-1/3 items-center">
                <input type="text" placeholder="Buscar en el mapa"
                className="outline-none flex-1"
                value={searcher}  
                onChange={e=>setSearcher(e.target.value)}/>   
                <button className="flex items-center justify-center"
                onClick={()=>{
                    async function getData() {
                        const data = (await getCoors(searcher)).data.features[0].center
                        setViewport({
                            longitude:  data[0],
                            latitude: data[1],
                            zoom: 14,
                            pitch: 40
                        })
                        setSearcher("");
                    }
                    getData()
                }}
                >
                    <span className="material-symbols-outlined text-blue-600">
                        search
                    </span>         
                </button>
            </div>

            
        </div>
    )
}