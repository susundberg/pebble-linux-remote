

Pebble.addEventListener("ready", function(e) {
  console.log("NetDownload JS Ready");
});



Pebble.addEventListener("appmessage", function(e) {
  console.log("Got message: " + JSON.stringify(e));
});


