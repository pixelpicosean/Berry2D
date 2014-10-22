// Make 'window' the global scope
self = window = this;
window.top = window.parent = window;

(function(window) {
    // RAF & Timer implementation
    var animFrameCallbacks = [], rafRemoveList = [], nextAnimFrameCallbackID = 0;
    var timeoutCallbacks = [], timeoutRemoveList = [], nextTimeoutCallbackID = 0;
    var intervalCallbacks = [], intervalRemoveList = [], nextIntervalCallbackID = 0;
    __BERRY__.tickAnimFrame = function tickAnimFrame() {
        var i, len;
        for (i = 0, len = rafRemoveList.length; i < len; i++) {
            animFrameCallbacks.splice(animFrameCallbacks.indexOf(rafRemoveList[i]), 1);
        }
        rafRemoveList.length = 0;
        for (i = 0, len = animFrameCallbacks.length; i < len; i++) {
            animFrameCallbacks[i].callback();
        }

        var now = Date.now();
        // setTimeout
        for (i = 0, len = timeoutRemoveList.length; i < len; i++) {
            timeoutCallbacks.splice(timeoutCallbacks.indexOf(timeoutRemoveList[i]), 1);
        }
        timeoutRemoveList.length = 0;
        i = timeoutCallbacks.length - 1;
        do {
            if (i < 0) {
                break;
            }
            if (timeoutCallbacks[i].time < now) {
                timeoutCallbacks[i].callback();
                timeoutCallbacks.splice(i, 1);
            }
        }
        while (i--);
        // setInterval
        for (i = 0, len = intervalRemoveList.length; i < len; i++) {
            intervalCallbacks.splice(intervalCallbacks.indexOf(intervalRemoveList[i]), 1);
        }
        intervalRemoveList.length = 0;
        i = intervalCallbacks.length - 1;
        do {
            if (i < 0) {
                break;
            }
            if (intervalCallbacks[i].time < now) {
                intervalCallbacks[i].callback();
                intervalCallbacks[i].time = now + intervalCallbacks[i].interval;
            }
        }
        while (i--);
    };


    // window.devicePixelRatio = __BERRY__.devicePixelRatio;
    window.innerWidth = __BERRY__.screenWidth;
    window.innerHeight = __BERRY__.screenHeight;

    window.screen = {
        availWidth: window.innerWidth,
        availHeight: window.innerHeight
    };

    window.navigator = {
        language: __BERRY__.language,
        userAgent: __BERRY__.userAgent,
        appVersion: __BERRY__.appVersion,
        platform: __BERRY__.platform
    };

    // The console object
    window.console = {
        _log: function(level, args) {
            var txt = level + ':';
            for (var i = 0, len = args.length; i < len; i++) {
                txt += ' ' + (typeof args[i] === 'string' ? args[i] : JSON.stringify(args[i]));
            }
            print(txt);
        },

        assert: function() {
            var args = Array.prototype.slice.call(arguments);
            var assertion = args.shift();
            if (!assertion) {
                print('Assertion failed: ' + args.join(', '));
            }
        }
    };
    window.console.debug = function() { window.console._log('DEBUG', arguments); };
    window.console.info =  function() { window.console._log('INFO', arguments); };
    window.console.warn =  function() { window.console._log('WARN', arguments); };
    window.console.error = function() { window.console._log('ERROR', arguments); };
    window.console.log =   function() { window.console._log('LOG', arguments); };

    var consoleTimers = {};
    console.time = function(name) {
        consoleTimers[name] = Date.now();
    };

    console.timeEnd = function(name) {
        var timeStart = consoleTimers[name];
        if (!timeStart) {
            return;
        }

        var timeElapsed = Date.now() - timeStart;
        console.log(name + ': ' + timeElapsed + 'ms');
        delete consoleTimers[name];
    };

    // Timers
    window.performance = {
        now: function() {
            return Date.now();
        }
    };
    window.setTimeout = function(cb, t) {
        var id = nextTimeoutCallbackID++;
        timeoutCallbacks.push({
            id: id,
            callback: cb,
            time: Date.now() + t
        });
        return id;
    };
    window.setInterval = function(cb, t) {
        var id = nextIntervalCallbackID++;
        intervalCallbacks.push({
            id: id,
            callback: cb,
            time: Date.now() + t,
            interval: t
        });
        return id;
    };
    window.clearTimeout = function(id) {
        var i = timeoutCallbacks.length - 1;
        do {
            if (i < 0) {
                break;
            }
            if (timeoutCallbacks[i].id === id) {
                // timeoutCallbacks.splice(i, 1);
                timeoutRemoveList.push(timeoutCallbacks[i]);
                return;
            }
        }
        while (i--);
    };
    window.clearInterval = function(id) {
        var i = intervalCallbacks.length - 1;
        do {
            if (i < 0) {
                break;
            }
            if (intervalCallbacks[i].id === id) {
                // intervalCallbacks.splice(i, 1);
                intervalRemoveList.push(intervalCallbacks[i]);
                return;
            }
        }
        while (i--);
    };
    window.requestAnimationFrame = function(cb) {
        var id = nextAnimFrameCallbackID++;
        animFrameCallbacks.push({
            id: id,
            callback: cb
        });
        return id;
    };
    window.cancelAnimationFrame =function(id) {
        for (var i = 0, len = animFrameCallbacks.length; i < len; i++) {
            if (animFrameCallbacks[i].id === id) {
                // animFrameCallbacks.splice(i, 1);
                rafRemoveList.push(animFrameCallbacks[i]);
                return;
            }
        }
    };

})(this);
