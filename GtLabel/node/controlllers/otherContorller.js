var fs = require('fs');
const moment = require('moment');

exports.update = (req, res) => {
    let obj = {};
    obj["cmd"] = "update";
    try {
        let readDir = fs.readdirSync(__dirname + "/pack");
        let arryLinux = [];
        let arryWin = [];
        readDir.forEach(element => {
            const result = fs.statSync(__dirname + "/pack/" + element);
            if (element.search("linux") != -1) {
                arryLinux.push({ ctime: result.ctime, version: element });
            }
            if (element.search("win") != -1) {
                arryWin.push({ ctime: result.ctime, version: element });
            }
        });
    
        arryLinux.sort(function (a, b) {
            return a.ctime < b.ctime;
        });
        arryWin.sort(function (a, b) {
            return a.ctime < b.ctime;
        });
    
        if (arryLinux.length > 0) {
            arryLinux[0].ctime = moment(arryLinux[0].ctime).format('YYYY-MM-DD HH:mm:ss')
            var linuxVersion = arryLinux[0];
            obj["linux"] = linuxVersion
        }
        if (arryWin.length > 0) {
            arryWin[0].ctime = moment(arryWin[0].ctime).format('YYYY-MM-DD HH:mm:ss')
            var winVersion = arryWin[0];
            obj["win"] = winVersion
        }
    } catch (error) {
        
    }
   
    res.json(obj);
    res.end();
}