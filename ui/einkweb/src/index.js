/*jshint node:false */
"use strict";


(function() {

/*

*/


    var layout = new FlowLayout(3,2);
    layout.setPageTitle("Engine");
    layout.append(EInkTextBox.number("1.engine.0.speed","engine","rpm",0));
    layout.append(EInkTextBox.number("1.engine.0.altenatorVoltage","alternator","V"));
    layout.append(EInkTextBox.number("1.engine.0.hours","hours","h", 2, 1/3600));
    layout.append(EInkTextBox.number("1.engine.0.status1","engine","status1", 0));
    layout.append(EInkTextBox.number("1.engine.0.status1","engine","status1", 0));
    layout.append(EInkTextBox.number("1.fluidLevel.0.level","fuel","%",0));
    // No need to call new page as its full already.
    //layout.newPage();
    layout.setPageTitle("Batteries");
    layout.append(EInkTextBox.number("1.battery.0.voltage","engine","V"));
    layout.append(EInkTextBox.number("1.battery.1.voltage","service","V"));
    layout.append(EInkTextBox.number("1.battery.2.voltage","alternator","V"));
    layout.append(EInkTextBox.number("1.battery.3.voltage","other","V"));
    layout.newPage();
    layout.setPageTitle("Pressure");
    layout.append(EInkTextBox.number("7.bmp280.pressure","bmp280","mbar", 1));
    layout.append(EInkTextBox.number("7.bmp280.temp","bmp280","C",1));
    layout.append(EInkTextBox.number("7.bmp280.humidity","bmp280","%RH",1));




/*
                new EInkRelativeAngle('environment.wind.angleApparent','awa', c(),r()),
                new EInkSpeed('environment.wind.speedApparent', 'aws',        c(),r()),
                new EInkSpeed('navigation.speedThroughWater', 'stw',          c(),r()),
                new EInkDistance('environment.depth.belowTransducer', 'dbt',  c(),r()),

                new EInkRelativeAngle('environment.wind.angleTrueWater','twa',c(),r()),
                new EInkSpeed('environment.wind.speedTrue', 'tws',            c(),r()),
                new EInkRelativeAngle('performance.leeway','leeway',c(),r(), undefined, 1),
                new EInkAttitude(c(),r()),

                new EInkSpeed('navigation.speedOverGround', 'sog',            c(),r()),
                new EInkBearing('navigation.courseOverGroundMagnetic', 'cogm',c(),r()),
                new EInkPossition(c(),r()),
                new EInkLog(c(),r()),

                new EInkCurrent(c(),r()),
                new EInkPilot(c(),r()),
                new EInkFix(c(),r()),
                new EInkTemperature('environment.water.temperature','water',c(),r()),

                new EInkBearing('environment.wind.windDirectionMagnetic', 'windM',c(),r()),
                new EInkSpeed('performance.velocityMadeGood', 'vmg',          c(),r()),
                new EInkSys(c(),r()),
                new EInkBearing('navigation.headingMagnetic','hdm',c(),r()),

                new EInkSpeed('performance.polarSpeed', 'polar stw',        c(),r()),
                new EInkSpeed('performance.vmg', 'polar vmg',        c(),r()),
                new EInkSpeed('performance.polarVmg', 'best polar vmg',        c(),r()),
                new EInkSpeed('performance.targetStw', 'target stw',        c(),r()),

                new EInkSpeed('performance.targetVmg', 'target vmg',        c(),r()),
                new EInkBearing('performance.oppositeTrackMagnetic', 'op tack m',c(),r()),
                new EInkBearing('performance.oppositeHeadingMagnetic', 'op head m',c(),r()),
                new EInkRelativeAngle('performance.targetTwa','target twa',c(),r()),

                new EInkRatio('performance.polarSpeedRatio',"polar stw perf", c(), r()),
                new EInkRatio('performance.polarVmgRatio',"polar vmg perf", c(), r())
*/

        var themes = {
            "day": {
                foreground: "black",
                background: "white"
            },
            "night" : {
                foreground: "white",
                background: "black"

            },
            "nightred" : {
                foreground: "red",
                background: "black"
            },
            "nightvision" : {
                foreground: "green",
                background: "black"
            }
        };
        var isPortrait = !(Object.create === undefined);
        var drawingOptions = {
            canvas: document.getElementById("canvas"),            
            themes: themes,
            portrait: !isKindle, 
            theme: "night",
            layout: layout
        };
        var drawingContext = new EInkDrawingContext(drawingOptions);

        var updater =  new EInkUpdater({
            url: "http://boatsystems.local/api/data/all",
            calculations: new Calcs(),
            context: drawingContext,
            period: 5000
        });
        updater.update();

        var uiController = new EInkUIController({
            context: drawingContext,
            rotateControl: document.getElementById("rotate"),
            pageControl: document.getElementById("page"),
            themeControl: document.getElementById("theme"),
            rotations: [ "portrate", "landscape"],
            themes: ["day","night","nightred","nightvision"]
        });


        document.getElementById("body").addEventListener("keydown", function(e) {
            debug("got keydown");
        });
        document.getElementById("body").addEventListener("auxclick", function(e) {
            debug("got auxclick");
        });
        document.getElementById("body").addEventListener("keypress", function(e) {
            debug("got keypress");
        });
        document.getElementById("body").addEventListener("scroll", function(e) {
            debug("got scroll");
        });



})();

