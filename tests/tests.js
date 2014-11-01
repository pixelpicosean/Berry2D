describe('window', function() {
    it('has numeric property innerWidth and innerHeight', function() {
        expect(window.innerWidth).toEqual(jasmine.any(Number));
        expect(window.innerHeight).toEqual(jasmine.any(Number));
    });
    it('has setTimeout method', function() {
        expect(window.setTimeout).toBeDefined();
    });
    it('has setInterval method', function() {
        expect(window.setInterval).toBeDefined();
    });
    it('has RAF support', function() {
        expect(window.requestAnimationFrame).toBeDefined();
    });
});

describe('window.navigator', function() {
    it('has language info', function() {
        expect(window.navigator.language).toEqual(jasmine.any(String));
    });
    it('has userAgent info', function() {
        expect(window.navigator.userAgent).toEqual(jasmine.any(String));
    });
    it('has appVersion info', function() {
        expect(window.navigator.appVersion).toEqual(jasmine.any(String));
    });
    it('has platform info', function() {
        expect(window.navigator.platform).toEqual(jasmine.any(String));
    });
});

describe('window.console', function() {
    it('has debug method', function() {
        expect(console.debug).toBeDefined();
    });
    it('has info method', function() {
        expect(console.info).toBeDefined();
    });
    it('has error method', function() {
        expect(console.error).toBeDefined();
    });
    it('has warn method', function() {
        expect(console.warn).toBeDefined();
    });
    it('has log method', function() {
        expect(console.log).toBeDefined();
    });
});

describe('window.Date', function() {
    it('should exists', function() {
        expect(window.Date).toBeDefined();
    });
    it('returns current date with now()', function() {
        expect(Date.now()).toEqual(jasmine.any(Number));
    });
});

describe('window.Math', function() {
    it('has method "sin"', function() {
        expect(Math.sin).toBeDefined();
    });
    it('has method "cos"', function() {
        expect(Math.cos).toBeDefined();
    });
    it('has method "tan"', function() {
        expect(Math.tan).toBeDefined();
    });
    it('has method "atan2"', function() {
        expect(Math.atan2).toBeDefined();
    });
    it('has constant "PI"', function() {
        expect(Math.PI).toBeCloseTo(3.141593);
    });
    it('random() returns number between 0 and 1', function() {
        var result = Math.random();
        expect(result).toBeGreaterThan(0);
        expect(result).toBeLessThan(1);
    });
});

describe('localStorage', function() {
    it('is available', function() {
        expect(localStorage).toBeDefined();
    });
    it('can save string value', function() {
        var saveTest = function saveTest() {
            var value = '10';
            localStorage.setItem('myHealth', value);
        };
        expect(saveTest).not.toThrow();
    });
    it('can gives stored value as string', function() {
        var value = '100';
        localStorage.setItem('myMagic', value);
        expect(localStorage.getItem('myMagic')).toEqual(jasmine.any(String));
        expect(localStorage.getItem('myMagic')).toEqual(value);
    });
    it('saves number as string', function() {
        var value = 10;
        localStorage.setItem('myHealth', value);
        expect(localStorage.getItem('myHealth')).toEqual(value.toString());
    });
    it('has removeItem method', function() {
        expect(localStorage.removeItem).toBeDefined();

        var value = '100';
        localStorage.setItem('myMagic', value);
        describe('and will remove expected item', function() {
            beforeEach(function() {
                localStorage.removeItem('myMagic');
            });
            it('returns undefined after remove', function() {
                expect(localStorage.getItem('myMagic')).toBeUndefined();
            });
        });
    });
    it('has clear method', function() {
        expect(localStorage.clear).toBeDefined();

        describe('and will clear all saved items', function() {
            beforeEach(function() {
                localStorage.setItem('key1', 'value1');
                localStorage.setItem('key2', 'value2');
                localStorage.setItem('key3', 'value3');
            });
            it('clears every saved item', function() {
                expect(localStorage.getItem('key1')).toBeUndefined();
                expect(localStorage.getItem('key2')).toBeUndefined();
                expect(localStorage.getItem('key3')).toBeUndefined();
            });
        })
    });
});

describe('Image class', function() {
    var img;
    beforeEach(function() {
        img = new Image();
    });

    it('is defined global', function() {
        expect(window.Image).toBeDefined();
    });
    it('has nodeName', function() {
        expect(img.nodeName).toEqual(jasmine.any(String));

        describe('and nodeName is constant', function() {
            beforeEach(function() {
                img.nodeName = 'MY_IMG';
            });
            it('always equal to IMG', function() {
                expect(img.nodeName).toEqual('IMG');
            });
        });
    });
    it('will fire load event', function() {
        var loadCallback;
        beforeEach(function(done) {
            loadCallback = jasmine.createSpy('loadCallback');
            img.addEventListener('load', function() {
                loadCallback();
                done();
            });

            img.src = 'assets/heart.png';
        });
        it('fires load event after src is set', function() {
            expect(loadCallback).toHaveBeenCalled();
        });
    });
});

// Run specs
jasmine.getEnv().execute();
