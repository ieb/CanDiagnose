$fn=100;
dw=96.5;
dd=57.5;
w=dw+20;
d=dd+8;
r=3;
h=7;
difference() {
    union() {
        translate([0,r,0])
        cube([w,d-2*r,h]);
        translate([r,0,0])
        cube([w-2*r,d,h]);
        translate([r,r,0])
        cylinder(r=r,h=h);
        translate([w-r,r,0])
        cylinder(r=r,h=h);
        translate([w-r,d-r,0])
        cylinder(r=r,h=h);
        translate([r,d-r,0])
        cylinder(r=r,h=h);
        
        
    }
    translate([4,4,-1])
        cube([dw,dd,h+2]);
    
    translate([dw+8,8,-1])
    cube([6.5,6.5,10]);
    translate([dw,4,-1])
    cube([17,15,3]);


}