@ECHO OFF
REM convert -crop "$x_width"x"$y_height"+$x0+$y0 -resize $resolution -type TrueColorMatte -depth 8 -quality 100 "$outdir/temp_slideshow_image_scaled.ppm" "$outdir"/temp_slideshow_image.ppm	
REM 651,390;1134,759;372,330;1365,1089

REM xs_width=1134-651=483, xs_height=759-390=369
REM xe_width=1365-372=993, xe_height=1089-330=759

convert -crop 483x369+651+390 -type TrueColorMatte -depth 8 -quality 100 input.jpg out_xs.jpg
convert -crop 483x369+651+390 -resize 720x480 -type TrueColorMatte -depth 8 -quality 100 input.jpg out_xs_resize.jpg
convert -crop 483x369+651+390 -resize 720x480! -type TrueColorMatte -depth 8 -quality 100 input.jpg out_xs_resize_force.jpg

convert -crop 993x759+372+330 -type TrueColorMatte -depth 8 -quality 100 input.jpg out_xe.jpg
convert -crop 993x759+372+330 -resize 720x480 -type TrueColorMatte -depth 8 -quality 100 input.jpg out_xe_resize.jpg
convert -crop 993x759+372+330 -resize 720x480! -type TrueColorMatte -depth 8 -quality 100 input.jpg out_xe_resize_force.jpg

PAUSE
