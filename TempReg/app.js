function spasiPodatke()
{
    tempZraka=Number(document.getElementById('tempInput').value); 
    firebase.database().ref('parametri').set({
        parametarTemperatura: tempZraka,
            }, (error) => {
                if (error) {
            console.log("Greska u snimanju podataka!");
                    } else {
            console.log("Uspjesno snimljeni podaci!");
        }
});
}

function svijetli()
{
    tempZraka=Number(document.getElementById('tempInput').value); 
    firebase.database().ref('parametri').set({
        parametarTemperatura: tempZraka,
            }, (error) => {
                if (error) {
            console.log("Greska u snimanju podataka!");
                    } else {
            console.log("Uspjesno snimljeni podaci!");
        }
});
}
