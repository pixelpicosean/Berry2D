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
