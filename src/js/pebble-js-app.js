function init(e) {
    Pebble.addEventListener('showConfiguration')
    {
        console.log('Setting configuration event listener');
    }
}

Pebble.addEventListener('ready', init);
