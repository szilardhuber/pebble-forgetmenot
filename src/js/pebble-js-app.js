function init(e) {
    Pebble.addEventListener('showConfiguration', function(e)
    {
        console.log('Setting configuration event listener');
        Pebble.openURL("http://szilardhuber.github.io/pebble-forgetmenot/");
    });

    Pebble.addEventListener('webviewclosed', function(e)
    {
        var configuration = JSON.parse(decodeURIComponent(e.response));
        console.log('Configuration window returned: ', JSON.stringify(configuration));
        Pebble.sendAppMessage( {"text": configuration.messageText, "time": configuration.time},
        function(e) {
                console.log('Successfully delivered message with transactionId='
                + e.data.transactionId);
            },
            function(e) {
                console.log('Unable to deliver message with transactionId='
                + e.data.transactionId
                + ' Error is: ' + e.error.message);
            }
        );
    });
}

Pebble.addEventListener('ready', init);
