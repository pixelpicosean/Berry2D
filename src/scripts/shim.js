__MURAL__.__defineSetter__ = function __defineSetter__(obj, key, set) {
    Object.defineProperty(obj, key, {
        enumerable: true, configurable: true,
        set: set
    });
};

__MURAL__.__defineGetter__ = function __defineGetter__(obj, key, get) {
    Object.defineProperty(obj, key, {
        enumerable: true, configurable: true,
        get: get
    });
};

__MURAL__.__defineAccessor__ = function __defineAccessor__(obj, key, set, get) {
    Object.defineProperty(obj, key, {
        enumerable: true, configurable: true,
        set: set, get: get
    });
};

__MURAL__.Event = function(type) {
    this.type = type;
    this.cancelBubble = false;
    this.cancelable = false;
    this.target = null;

    this.initEvent = function(type, bubbles, cancelable) {
        this.type = type;
        this.cancelBubble = bubbles;
        this.cancelable = cancelable;
    };

    this.preventDefault = function() {};
    this.stopPropagation = function() {};
};
__MURAL__.eventMixin = (function() {
    function createEvent(type) {
        return new __MURAL__.Event(type);
    }
    function addEventListener(type, callback, useCapture) {
        if (!this.events[type]) {
            this.events[type] = [];
        }
        this.events[type].push(callback);
    }
    function removeEventListener(type, callback) {
        var listeners = this.events[type];
        if (!listeners) {
            return;
        }

        for (var i = listeners.length; i--;) {
            if (listeners[i] === callback) {
                listeners.splice(i, 1);
            }
        }
    }
    function dispatchEvent(event) {
        var listeners = this.events[event.type];
        // Call onevent method if exists
        if (typeof this['on' + event.type] === 'function') {
            this['on' + event.type](event);
        }
        if (!listeners) {
            return;
        }

        for (var i = 0; i < listeners.length; i++) {
            listeners[i](event);
        }
    }

    return function() {
        this.events = {};
        this.addEventListener = addEventListener;
        this.removeEventListener = removeEventListener;
        this.dispatchEvent = dispatchEvent;

        return this;
    };
})();
