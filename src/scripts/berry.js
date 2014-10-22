// Make 'window' the global scope
self = window = this;
window.top = window.parent = window;

(function(window) {
    // RAF implementation
    var animFrameCallbacks = [], nextAnimFrameCallbackID = 0;
    __BERRY__.tickAnimFrame = function tickAnimFrame() {
        for (var i = 0, len = animFrameCallbacks.length; i < len; i++) {
            animFrameCallbacks[i].callback();
        }
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

    window.requestAnimationFrame = function(cb) {
        animFrameCallbacks.push({
            id: nextAnimFrameCallbackID++,
            callback: cb
        });
        return nextAnimFrameCallbackID;
    };
    window.cancelAnimationFrame =function(id) {
        for (var i = 0, len = animFrameCallbacks.length; i < len; i++) {
            if (animFrameCallbacks[i].id === id) {
                animFrameCallbacks.splice(i, 1);
            }
        }
    };

})(this);
