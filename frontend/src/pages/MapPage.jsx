import React, { useState, useEffect } from 'react';
import { useLoaderData } from "react-router-dom";
import Map, { Marker, Popup } from 'react-map-gl';
import 'mapbox-gl/dist/mapbox-gl.css';

import Filters from '../components/Filters';
import OffcanvaCan from '../components/OffcanvaCan';
import { getTrashCans, getCoors, getResidues} from '../js/api';
import { getColorCan } from '../js/helpers';
import { accessToken } from 'mapbox-gl';

const loader = async () => {
  const trashCans = (await getTrashCans()).data
  const residues = (await getResidues()).data
  return {trashCans, residues};
}


function MapPage() {
  
  const data = useLoaderData();

  const [viewstate, setViewstate] = useState({
    longitude:  -77.082995,
    latitude: -12.055431,
    zoom: 14,
    pitch: 40,
  });

  
  const [trashCans, setTrashCans] = useState(data.trashCans);
  const [residues, setResidues] = useState(data.residues);
  
  const [selectedMarker, setSelectedMarker] = useState(null);
  
  const handleMarkerClick = (marker) => {    
    setSelectedMarker(selectedMarker!==null?null:marker);
  };
  
  return (
    <div className='w-full h-full'>
      <Filters setViewport={setViewstate} />

      {Object.keys(data).length!==0 && 
        <Map
        interactive={true}
        dragPan={true}
        scrollZoom={true}
        
        {...viewstate}
        maxBounds={[ 
          [-87.477048, -21.303168],
          [-64.390605, 7.477464] 
        ]} 
        onMove={e=>setViewstate(e.viewState)} 
        mapStyle="mapbox://styles/mapbox/streets-v11"
        mapboxAccessToken='pk.eyJ1IjoibHVpcy1sdCIsImEiOiJjbHJzMDhvaDYwMXA4MmlwZXdkbjBlbGJ4In0.kp9VsLm8L1UodQUws7ImPg'
        >
          {trashCans.map((can) => (    
            <Marker
            key={can.id}
            latitude={can.latitude}
            longitude={can.longitude} 
            offsetLeft={-20}
            offsetTop={-10}
            onClick={() => handleMarkerClick(can)}
            >
              <img src="/logo.png" alt="logo" width={30}/>
            </Marker>
          ))}        
        </Map> 
      } 
      {selectedMarker!==null &&  <OffcanvaCan
        selectedCan={selectedMarker}
        setSelectedCan={setSelectedMarker}
        residues={residues}
      />
      }
    </div>
  )
}

export default MapPage

export {loader};