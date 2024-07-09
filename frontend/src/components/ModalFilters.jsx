import { useEffect, useState } from "react";
import { findObjectByID } from "../js/helpers";

export default function ModalFilters({setModalFilters, 
    selectedCrimenes, 
    setSelectedCrimenes, 
    crimenes,
    ciudades,
    distritos,
    selectedCiudades,
    setSelectedCiudades,
    selectedDistritos,
    setSelectedDistritos
    }) {
 
    return (
        <div className="bg-white w-1/3 absolute top-0 right-0 z-20 h-full p-4 overflow-scroll">
            <div className="flex justify-end">
                <button onClick={()=>{setModalFilters(false)}}>
                    <span className="material-symbols-outlined">
                        close
                    </span>
                </button>
            </div>
            <p className="font-bold text-2xl mb-2">Filtrar por delito:</p>
            <div className="w-full flex flex-wrap">
                {crimenes.map((crimen) => {
                    return <div className="w-1/3 flex items-center gap-1 my-3"
                    key={crimen.id}> 
                        <label htmlFor={crimen.name}>{crimen.name}</label>
                        <input type="checkbox" id={crimen.name}
                        value={crimen.name}
                        checked={findObjectByID(crimen.id, selectedCrimenes)}
                        onChange={()=>{ 
                            if (findObjectByID(crimen.id, selectedCrimenes)) {
                                setSelectedCrimenes(selectedCrimenes.filter((c) => c.id !== crimen.id));
                            } else {
                                setSelectedCrimenes([...selectedCrimenes, crimen]);
                            }
                        }} 
                        />
                    </div>
                })}
            </div>
            <p className="font-bold text-2xl mb-2">Filtrar por lugar:</p>
            <div className="w-full"> 
                {ciudades.map(ciudad=>{
                    return <div className="w-full my-3" key={ciudad.id}>
                        <div className="w-full flex gap-1">
                            <label className="text-xl font-bold" htmlFor={ciudad.name}>{ciudad.name}</label>
                            <input type="checkbox" id={ciudad.name}
                                value={ciudad.name}
                                checked={findObjectByID(ciudad.id, selectedCiudades)}
                                onChange={()=>{ 
                                    if (findObjectByID(ciudad.id, selectedCiudades)) {
                                        setSelectedCiudades(selectedCiudades.filter((c) => c.id !== ciudad.id));
                                        setSelectedDistritos(selectedDistritos.filter((d) => d.city !== ciudad.id))
                                    } else {
                                        setSelectedCiudades([...selectedCiudades, ciudad]);
                                        setSelectedDistritos([...selectedDistritos, ...(distritos.filter(d=>d.city===ciudad.id))])
                                    }
                                }} 
                            />
                        </div>
                        <div className="w-full flex items-center justify-between flex-wrap">
                            {distritos.map(distrito=>{
                                if (distrito.city === ciudad.id) {
                                    return <div className="w-1/3 flex items-center gap-1 my-1"
                                    key={distrito.id}>
                                    <label htmlFor={distrito.name}>{distrito.name}</label>
                                    <input type="checkbox" id={distrito.name}
                                    value={distrito.name}
                                    checked={findObjectByID(distrito.id, selectedDistritos)}
                                    onChange={()=>{ 
                                        if (findObjectByID(distrito.id, selectedDistritos)) {
                                            setSelectedDistritos(selectedDistritos.filter((d) => d.id !== distrito.id));
                                        } else {
                                            setSelectedDistritos([...selectedDistritos, distrito]);
                                        }
                                    }} 
                                    />
                                    </div>
                                }
                                return
                            })}
                        </div>
                    </div>
                })}
            </div>
        </div>
    )
}