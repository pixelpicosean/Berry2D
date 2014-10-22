// Make 'window' the global scope
self = window = this;
window.top = window.parent = window;

(function(window) {

    // window.devicePixelRatio = __BERRY__.devicePixelRatio;
    window.innerWidth = __BERRY__.screenWidth;
    window.innerHeight = __BERRY__.screenHeight;

    var animFrameCallbacks = [], nextAnimFrameCallbackID = 0;
    __BERRY__.tickAnimFrame = function tickAnimFrame() {
        for (var i = 0, len = animFrameCallbacks.length; i < len; i++) {
            animFrameCallbacks[i].callback();
        }
    };

    // API
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
