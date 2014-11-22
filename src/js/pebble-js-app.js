function init(e) {
    Pebble.addEventListener('showConfiguration', function(e)
    {
        console.log('Setting configuration event listener');
        Pebble.openURL("https://raw.githubusercontent.com/szilardhuber/pebble-forgetmenot/master/src/js/settings.html");
    });
}

Pebble.addEventListener('ready', init);
