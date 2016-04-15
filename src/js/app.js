var Clay = require('clay');
var config = require('config.json');
var apliteConfig = require('config-aplite.json');
var clay = new Clay(config, null, { autoHandleEvents: false });

Pebble.addEventListener('showConfiguration', function(e) {
    var platform = clay.meta.activeWatchInfo.platform || 'aplite';
    if (platform === 'aplite') {
        clay.config = apliteConfig;
    }

    Pebble.openURL(clay.generateUrl());
});

Pebble.addEventListener('webviewclosed', function(e) {
      if (e && !e.response) {
        return;
    }

    // Get the keys and values from each config item
    var dict = clay.getSettings(e.response);

    // Send settings values to watch side
    Pebble.sendAppMessage(dict, function(e) {
        console.log('Sent config data to Pebble');
    }, function(e) {
        console.log('Failed to send config data!');
        console.log(JSON.stringify(e));
    });
});
