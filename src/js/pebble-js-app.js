function init(e) {
    Pebble.addEventListener('showConfiguration', function(e)
    {
        console.log('Setting configuration event listener');
        Pebble.openURL("http://szilardhuber.github.io/pebble-forgetmenot/");
    });
}

Pebble.addEventListener('ready', init);
