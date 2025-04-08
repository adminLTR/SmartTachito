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
    const m = d.getMonth() + 1;
    const f = d.getDate();
    return `${m < 10 ? '0'+m : m}-${f < 10 ? '0'+f : f}`;
}

export function parseHour(date) { 
    const d = new Date(date);
    return `${d.getHours()}:${d.getMinutes()}`;
}

export function getDataset(data) {
    const dataset = [];
    const labels = [];
    let sum = 0;
    data.forEach(residue => {
        let index = labels.indexOf(residue.most_confident_label);
        if (index!=-1) {
            dataset[index]++;
        } else {
            labels.push(residue.most_confident_label);
            dataset.push(1);
        }
        sum++;
    });
    return [labels, dataset.map(q => q*100/sum)];
}
/**
 * 
 * @param {Date | string} today 
 */
export function getWeek(today) {
    const week = [];
    today = new Date(today);
    for (let i = 6; i >= 0; i--) {
        const x = new Date(today);
        x.setDate(x.getDate() - i);
        week.push(x);
    }
    return week;
}

export function getDatasetWeek(residues) {
    const week = getWeek(new Date());
    const dataset = [];
    residues.forEach(residue => {
        let index = labels.indexOf(residue.most_confident_label);
        if (index!=-1) {
            dataset[index]++;
        } else {
            dataset.push({label:residue.most_confident_label, data:[0,0,0,0,0,0,0]});
        }
        sum++;
    });
    residues.forEach(r => {
        const date = new Date(r.datetime);
        let index = -1;
        for (let i = 0; i < week.length; i++) {
            if (equalsDates(week[i], date)) {
                index = i;
                break;
            }
        }
        if (index!==-1) {
            dataset[index]++;
        }
    })
}

/**
 * 
 * @param {Date | string} date1 
 * @param {Date | string} date2 
 */
export function equalsDates(date1, date2) {
    return date1.getFullYear()===date2.getFullYear() && date1.getDate()===date2.getDate() && date1.getMonth()===date2.getMonth();
}