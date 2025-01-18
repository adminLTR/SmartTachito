import { useEffect, useState } from "react";
import { getResiduesCan } from "../js/api";
import { Pie, Line } from 'react-chartjs-2';
import { getDataset, getWeek, parseDate } from "../js/helpers";
import {Chart as ChartJS} from 'chart.js/auto';


export default function OffcanvaCan (
    {selectedCan, setSelectedCan, residues}
    ) {

    const [residuesFilter, setResiduesFilter] = useState(residues.filter(r => r.trash_can == selectedCan.id))

    return (
        <div className="bg-white w-1/3 absolute top-0 right-0 z-20 h-full p-4 overflow-scroll">
            <div className="flex justify-end">
                <button onClick={()=>{setSelectedCan(null)}}>
                    <span className="material-symbols-outlined">
                        close
                    </span>
                </button>
            </div>
            <div className="w-full">
                {/* <h4 className="text-2xl">
                    {crimen.name}
                </h4> */}
                <div className="rounded w-2/3 m-auto border-b-2 p-4">
                    <h3 className="text-center uppercase">
                        % total de residuos
                    </h3>
                    {residuesFilter.length!==0 && <Pie
                        data={{
                            labels: getDataset(residuesFilter)[0],
                            datasets:[{
                                data: getDataset(residuesFilter)[1],
                                hoverOffset: 4,
                            }]
                        }}
                    />
                    }
                </div>
                <div className="rounded border-b-2 p-4">
                    <h3 className="text-center uppercase w-2/3 m-auto">
                        Residuos detectados en la última semana
                    </h3>
                    {residuesFilter.length!==0 && <Line
                        data={{
                            labels: getWeek(new Date()).map(date => parseDate(date)),
                            datasets: [
                                {
                                    data: getWeek(new Date()).map(date => Math.floor(Math.random()*(20))),
                                    borderColor: "red",
                                    label: "Plastico"
                                },
                                {
                                    data: getWeek(new Date()).map(date => Math.floor(Math.random()*(20))),
                                    borderColor: "green",
                                    label: "Papel"
                                },
                                {
                                    data: getWeek(new Date()).map(date => Math.floor(Math.random()*(20))),
                                    borderColor: "blue",
                                    label: "General"
                                },
                            ]
                        }}
                    />}
                </div>
            </div>
            <div className="w-full">
                {/* <div className="descripcion">
                    <h5 className="font-bold">Descripcion: </h5>
                    <p>{selectedCan.crime_detail}</p>
                    <p>{selectedCan.comment}</p>
                </div> */}
                <div className="data-crime my-2">
                    <p><span className="font-bold">Longitud:</span> {selectedCan.longitude}</p>
                    <p><span className="font-bold">Latitud:</span> {selectedCan.latitude}</p>
                    <p><span className="font-bold">Humedad:</span> {selectedCan.humidity}%</p>
                    <p><span className="font-bold">Temperatura:</span> {selectedCan.temperature}C°</p>
                </div>
            </div>
        </div>  
 
    )
}
