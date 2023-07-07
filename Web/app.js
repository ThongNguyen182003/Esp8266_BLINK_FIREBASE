var firebaseConfig = {
    apiKey: "AIzaSyDW8WZjl27_RNBq7qr4KZkxFcGkEKwVh1U",
    authDomain: "esp8266demo-34e4a.firebaseapp.com",
    databaseURL: "https://esp8266demo-34e4a-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "esp8266demo-34e4a",
    storageBucket: "esp8266demo-34e4a.appspot.com",
    messagingSenderId: "889007057616",
    appId: "1:889007057616:web:63d4ce62561dbd99768cd5",
    measurementId: "G-ETJVX3KT9C"
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);
var database = firebase.database();
var Led1Status;
var firebaseRef = firebase.database().ref().child("LedStatus");

function togglecheckbox(element) {
    if (element.checked) {
        firebaseRef.set(1);
        Led1Status = "0";
    } else {
        firebaseRef.set(0);
        Led1Status = "1";
    }
}