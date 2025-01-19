import axios from "axios";


// const url = "http://192.168.1.2:8000";
const url = "http://127.0.0.1:8000";

export function getResiduesCan(can) {
    return axios.get(`${url}/api/residues/${can}/`);
}
export function getResidues() {
    return axios.get(`${url}/residues/`);
}
export function getTrashCans() {
    return axios.get(`${url}/trash_cans/`);
}


export function getCoors(direccion) {
    const token = "pk.eyJ1IjoibHVpcy1sdCIsImEiOiJjbHJzMDhvaDYwMXA4MmlwZXdkbjBlbGJ4In0.kp9VsLm8L1UodQUws7ImPg"
    // return axios.get(`https://api.mapbox.com/geocoding/v5/mapbox.places/${encodeURIComponent(direccion)}.json?access_token=${token}`)
    return axios.get(`https://api.mapbox.com/geocoding/v5/mapbox.places/${encodeURIComponent(direccion)}.json?country=PE&limit=1&access_token=${token}`)
}  