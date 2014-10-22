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
