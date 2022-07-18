/*jshint node:false */
"use strict";


(function() {

/*

*/


    var layout = new FlowLayout(3,2);
    layout.setPageTitle("Engine");
    layout.append(EInkTextBox.number("1.engine.[instance==0].speed","engine","rpm",0));
    layout.append(EInkTextBox.number("1.engine.[instance==0].altenatorVoltage","alternator","V"));
    layout.append(EInkTextBox.number("1.engine.[instance==0].hours","hours","h", 2, 1/3600));
    layout.append(EInkTextBox.number("1.engine.[instance==0].coolantTemp","coolant","C", 1, 1,  -273.15));
    layout.append(new EInkEngineStatus("1.engine.[instance==0]"));
    layout.append(EInkTextBox.number("1.fluidLevel.[instance==0,fluidType==0,debug].level","fuel","%",0)); // should be fuel
    // No need to call new page as its full already.
    //layout.newPage();
    layout.setPageTitle("Batteries");
    layout.append(EInkTextBox.number("1.battery.[instance==0].voltage","engine","V"));
    layout.append(EInkTextBox.number("1.battery.[instance==1].voltage","service","V"));
    layout.append(EInkTextBox.number("1.engine.[instance==0].altenatorVoltage","alternator","V"));
    layout.append(EInkTextBox.number("1.battery.[instance==2].voltage","engine controls","V"));
    layout.newPage();
    layout.setPageTitle("Environment");
    layout.append(EInkTextBox.number("4.pressure.0.actual","n2k","mbar", 1, 0.01, 0));
    layout.append(EInkTextBox.number("4.pressure.0.actual","n2k","mbar", 1, 0.01, 0));
    layout.append(EInkTextBox.number("5.temperatureExt.0.actual","n2k","C", 1, 1, -273.15));
    layout.append(EInkTextBox.number("4.humidity.0.actual","n2k","%RH",1));


    layout.newPage();
    layout.setPageTitle("Temperature");
    layout.append(EInkTextBox.number("6.temperatures.0.temperature","Engine Room","C", 2));
    layout.append(EInkTextBox.number("6.temperatures.1.temperature","Exhaust","C", 2));
    layout.append(EInkTextBox.number("6.temperatures.2.temperature","Battery S","C", 2));
    layout.append(EInkTextBox.number("6.temperatures.3.temperature","Alternator","C", 2));


    layout.newPage();
    layout.setPageTitle("Voltages");
    layout.append(EInkTextBox.number("8.battery.0.voltage","service","V", 2));
    layout.append(EInkTextBox.number("8.voltages.0.current","service","A", 1));
    layout.append(EInkTextBox.number("8.voltages.1.voltage","engine","V", 2));
    layout.append(EInkTextBox.number("8.voltages.1.current","engine","A", 1));

    layout.newPage();
    layout.setPageTitle("Performance");
    layout.append(EInkTextBox.number("performance.polarSpeed", 'polar stw',"Kn"));
    layout.append(EInkTextBox.number("performance.vmg", 'polar vmg', "Kn"));
    layout.append(EInkTextBox.number("performance.polarVmg","best polar vmg","Kn"));
    layout.append(EInkTextBox.number("performance.targetStw","target stw","Kn"));




// 0 =  devices not this page 
// done 1 == can engine
// 2 == can nav data
// 3 == can boat data from 
// 4 == can env data
// 5 == can temperature data
// // 6 == temperature 1 wire sensors
// done 7 == pressure bpm280
// done 8 == voltages



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
            body: document.getElementsByTagName("body")[0],            
            themes: themes,
            portrait: !isKindle, 
            theme: "night",
            layout: layout
        };
        var drawingContext = new EInkDrawingContext(drawingOptions);
//        console.log("Using DataServer", window.DataServerAddress);

        var updater =  new EInkUpdater({
            url: `${window.DataServerAddress}/api/data/all.json`,
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

        drawingContext.render();

})();

