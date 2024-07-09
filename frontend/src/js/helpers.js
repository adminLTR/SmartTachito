/**
 * finds if an object exists in an array with an ID given in params
 * @param {number} objectId object's id
 * @param {Array} array list of objects
 * @returns wether the object exists in the array
 */
export function findObjectByID(objectId, array) {
    for (let i = 0; i < array.length; i++) {
        if (array[i].id === objectId) {
            return true;
        }
    }
    return false;
}

export function getColorCan() {
    const colours = [
        "#D80505",
        "#0069D1",
        "#7524F1",
        "#5B5B5B",
        "#07A6C2",
        "#7CB00E",
        "#ED33DC",
        "#1AF800"
    ]
    return colours[Math.floor(Math.random()*(colours.length))]
} 

export function verifyDate(begin, end, test) {
    const inicio = begin=='' ? null : new Date(begin);
    const fin = end=='' ? null : new Date(end);
    const prueba = test=='' ? null : new Date(test);
    const inicioAdjusted = inicio==null?null: new Date(inicio.getFullYear(), inicio.getMonth(), inicio.getDate());
    const finAdjusted = fin==null?null: new Date(fin.getFullYear(), fin.getMonth(), fin.getDate());
    const pruebaAdjusted = prueba==null?null: new Date(prueba.getFullYear(), prueba.getMonth(), prueba.getDate());
    if (inicioAdjusted==null && finAdjusted==null) {
        return true;
    } else if (inicioAdjusted==null) {
        return pruebaAdjusted<=finAdjusted;
    } else if (finAdjusted==null) {
        return pruebaAdjusted >= inicioAdjusted
    } else {
        return pruebaAdjusted >= inicioAdjusted && pruebaAdjusted <= finAdjusted;
    }
}

export function parseDate(date) {
    const d = new Date(date);
    return `${d.getDate()}-${d.getMonth()}-${d.getFullYear()}`;
}

export function parseHour(date) { 
    const d = new Date(date);
    return `${d.getHours()}:${d.getMinutes()}`;
}

export function getDataset(data) {
    const dataset = [];
    const labels = [];
    data.forEach(residue => {
        let index = labels.indexOf(residue.most_confident_label);
        if (index!=-1) {
            dataset[index]++;
        } else {
            labels.push(residue.most_confident_label);
            dataset.push(1);
        }
    });
    return [labels, dataset];
}
