$fn=100;
module board() {
    difference() {
    union() {
        cube([75,59,3], center=true);
        translate([-32,-4,6])
        cube([8,41,12], center=true);
        translate([32,-12,6])
        cube([8,25,12], center=true);
        translate([0,14,8])
        cube([55,28,21], center=true);
        translate([20,-18,-8])
        cylinder(d=8,h=8);
    }
    translate([-(75/2)+4.5,-(59/2)+3,0])
    cylinder(d=4,h=10,center=true);
    translate([-(75/2)+6.2,(59/2)-6.5,0])
    cylinder(d=4,h=10,center=true);
    translate([(75/2)-4.6,(59/2)-4.7,0])
    cylinder(d=4,h=10,center=true);
    translate([(75/2)-4.6,-(59/2)+3.3,0])
    cylinder(d=4,h=10,center=true);
    }
}

module lid(w=110,d=70,h=15,hb=20,t=3) {
            translate([0,0,hb-t+1]) 
      difference() {
        union() {
            difference() {
                union() {
                    cube([w-t*2,d,h], center=true);
                    cube([w,d-t*2,h], center=true);
                    translate([w/2-t,d/2-t,0])
                    cylinder(r=t,h=h, center=true);
                    translate([-(w/2-t),d/2-t,0])
                    cylinder(r=t,h=h, center=true);
                    translate([-(w/2-t),-(d/2-t),0])
                    cylinder(r=t,h=h, center=true);
                    translate([w/2-t,-(d/2-t),0])
                    cylinder(r=t,h=h, center=true);
                    

                }
                // inside
                translate([0,0,-t*1.5])
                    cube([w-t*2,d-t*2,h+t], center=true);
            translate([0,0,-hb+t]) 
                cables();
            }
            // pillars
            translate([0,0,-hb+t]) {
            translate([-(75/2)+4.5,-(59/2)+3,0])
                union() {
                    cylinder(d=6,h=24);
                }
            translate([-(75/2)+6.2,(59/2)-6.5,0])
                union() {
                    cylinder(d=6,h=24);
                }
            translate([(75/2)-4.6,(59/2)-4.7,0])
                union() {
                    cylinder(d=6,h=24);
                }
            translate([(75/2)-4.6,-(59/2)+3.3,0])
                union() {
                    cylinder(d=6,h=24);
                }
            }
            // fxings
            translate([w/2-2*t,d/2-2*t,0])
                    cylinder(r=5,h=h, center=true);
            translate([-(w/2-2*t),d/2-2*t,0])
                    cylinder(r=5,h=h, center=true);
            translate([-(w/2-2*t),-(d/2-2*t),0])
                    cylinder(r=5,h=h, center=true);
                    translate([w/2-2*t,-(d/2-2*t),0])
                    cylinder(r=5,h=h, center=true);
        }
        
            translate([w/2-2*t,d/2-2*t,0])
                    cylinder(d=3,h=h+10, center=true);
            translate([-(w/2-2*t),d/2-2*t,0])
                    cylinder(d=3,h=h+10, center=true);
            translate([-(w/2-2*t),-(d/2-2*t),0])
                    cylinder(d=3,h=h+10, center=true);
                    translate([w/2-2*t,-(d/2-2*t),0])
                    cylinder(d=3,h=h+10, center=true);
    }
}

module case(w=110,d=70,h=20,t=3) {
      difference() {
        union() {
            difference() {
                union() {
                    cube([w-t*2,d,h], center=true);
                    cube([w,d-t*2,h], center=true);
                    translate([w/2-t,d/2-t,0])
                    cylinder(r=t,h=h, center=true);
                    translate([-(w/2-t),d/2-t,0])
                    cylinder(r=t,h=h, center=true);
                    translate([-(w/2-t),-(d/2-t),0])
                    cylinder(r=t,h=h, center=true);
                    translate([w/2-t,-(d/2-t),0])
                    cylinder(r=t,h=h, center=true);
                    

                }
                // inside
                translate([0,0,t])
                    cube([w-t*2,d-t*2,h+t], center=true);
                cables();
            }
            // pillars
            translate([0,0,-h/2+t/2]) {
            translate([-(75/2)+4.5,-(59/2)+3,0])
                union() {
                    cylinder(d=6,h=7);
                    cylinder(d=3,h=9);
                }
            translate([-(75/2)+6.2,(59/2)-6.5,0])
                union() {
                    cylinder(d=6,h=7);
                    cylinder(d=3,h=9);
                }
            translate([(75/2)-4.6,(59/2)-4.7,0])
                union() {
                    cylinder(d=6,h=7);
                    cylinder(d=3,h=9);
                }
            translate([(75/2)-4.6,-(59/2)+3.3,0])
                union() {
                    cylinder(d=6,h=7);
                    cylinder(d=3,h=9);
                }
            }
            // fxings
            translate([w/2-2*t,d/2-2*t,0])
                    cylinder(r=5,h=h, center=true);
            translate([-(w/2-2*t),d/2-2*t,0])
                    cylinder(r=5,h=h, center=true);
            translate([-(w/2-2*t),-(d/2-2*t),0])
                    cylinder(r=5,h=h, center=true);
                    translate([w/2-2*t,-(d/2-2*t),0])
                    cylinder(r=5,h=h, center=true);
        }
        
            translate([w/2-2*t,d/2-2*t,0])
                    cylinder(d=3,h=h+10, center=true);
            translate([-(w/2-2*t),d/2-2*t,0])
                    cylinder(d=3,h=h+10, center=true);
            translate([-(w/2-2*t),-(d/2-2*t),0])
                    cylinder(d=3,h=h+10, center=true);
                    translate([w/2-2*t,-(d/2-2*t),0])
                    cylinder(d=3,h=h+10, center=true);
    }

}
module cables() {
        translate([-90,-15,10])
    rotate([0,90,0])
    cylinder(d=4,h=90);
    translate([-90,15,10])
    rotate([0,90,0])
    cylinder(d=4,h=90);

    translate([-90,0,10])
    rotate([0,90,0])
    cylinder(d=4,h=90);

    translate([0,-15,10])
    rotate([0,90,0])
    cylinder(d=7,h=90);
}

/*
difference() {
    union() {
        //board();
        case();
        lid();
          //cables();
    }
    cube([65,160,140], center=true);
}
*/

case();
//replid();