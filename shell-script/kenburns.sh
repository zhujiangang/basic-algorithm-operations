#command line: kenburns.sh input.jpg "651,390;1134,759;372,330;1365,1089" ./output
#command line: kenburns.sh input.jpg "900,900;1700,1700;1100,1100;2500,1700" ./output
		i=0
		frames_per_sec=2997  # in 1/100ths of a second
		resolution='720x480'
		verbosity=0
		duration[$i]=5
		frames=$(( $frames_per_sec * ${duration[$i]} / 100 ))
		file="$1"
		effect_params[$i]="$2"
		outdir="$3"

		convert -size "$resolution" xc:black -type TrueColorMatte \
		-depth 8 "$outdir"/slideshow_background.ppm
		## Ken Burns effect from starting point to ending point
		echo "# Doing KenBurns Effect...."
		
		
		# x0,y0 is the top left corner of the image
		# x1,y1 is the bottom right corner of the image
		# xs0,ys1 is the starting point for the top left corner, etc
		# xe1,ye1 is the ending point for the bottom right corner
		# textfile format is:  
		# file:duration:comment:kenburns:xs0,ys0;xs1,ys1;xe0,ye0;xe1,ye1
		
		
		xs0=`echo "${effect_params[$i]}" | awk -F';' '{print $1}' | awk -F',' '{print $1}'`
		ys0=`echo "${effect_params[$i]}" | awk -F';' '{print $1}' | awk -F',' '{print $2}'`
		xs1=`echo "${effect_params[$i]}" | awk -F';' '{print $2}' | awk -F',' '{print $1}'`
		ys1=`echo "${effect_params[$i]}" | awk -F';' '{print $2}' | awk -F',' '{print $2}'`
		
		# ending point:
		xe0=`echo "${effect_params[$i]}" | awk -F';' '{print $3}' | awk -F',' '{print $1}'`
		ye0=`echo "${effect_params[$i]}" | awk -F';' '{print $3}' | awk -F',' '{print $2}'`
		xe1=`echo "${effect_params[$i]}" | awk -F';' '{print $4}' | awk -F',' '{print $1}'`
		ye1=`echo "${effect_params[$i]}" | awk -F';' '{print $4}' | awk -F',' '{print $2}'`
		echo "params=$xs0,$ys0 ; $xs1,$ys1 ; $xe0,$ye0 ; $xe1,$ye1"
		
		xs_width=$(( $xs1 - $xs0 )) ; ys_height=$(( $ys1 - $ys0 ))
		xe_width=$(( $xe1 - $xe0 )) ; ye_height=$(( $ye1 - $ye0 ))
		echo "size=$xs_width,$ys_height    $xe_width,$ye_height"
		
		## do some error checking to make sure the effect parameters are ok?
		## first, rescale the initial picture so the resolution if fine enough
		## remember output resolution is $width x $height
		## I figure we want about twice that resolution at all times?
		
		[ $xs_width -lt $xe_width ] && zoomin=0 || zoomin=1
		factor=1  #  if effect seems "choppy", increase factor.  Slows down processing.
		factor_percent=$(( 100* $factor))  # in percent
		
		if [ $zoomin -eq 1 ] ; then
			# use end point resolution
			echo "height=$height ys_height=$ys_height factor=$factor"
			echo "zoomin"
			## crop to size of start point:
			convert -crop "$xs_width"x"$ys_height"+$xs0+$ys0 -resize "$factor_percent"% \
			-depth 8 -quality 100 "${file}" "$outdir"/temp_slideshow_image_scaled.ppm		
			## now the image is cropped to be the large size and resized by 2
			## reset / change the start and end points: (coordiate transformation by xs0,ys0
			xs0=0; ys0=0 ; 
			xs1=$(( $xs_width * $factor )); ys1=$(( $ys_height * $factor )); 
			xe0=$(( ($xe0-$xs0) * $factor )); ye0=$(( ($ye0-$ys0) * $factor )); 
			xe1=$(( ($xe1-$xs0) * $factor )); ye1=$(( ($ye1-$ys0) * $factor )); 
			echo "new params=$xs0,$ys0 ; $xs1,$ys1 ; $xe0,$ye0 ; $xe1,$ye1"
		else  # zoomout
			# use start point resolution
			echo "height=$height ys_height=$ys_height factor=$factor"
			echo "zoomout"
			## crop to size of end point:  
			convert -crop "$xe_width"x"$ye_height"+$xe0+$ye0 -resize "$factor_percent"% \
			-depth 8 -quality 100 "${file}" "$outdir"/temp_slideshow_image_scaled.ppm		
			## now the image is cropped to be the large size and resized by 2
			## reset / change the start and end points: (coordiate transformation by xe0,ye0
			xs0=$(( ($xs0-$xe0) * $factor )); ys0=$(( ($ys0-$ye0) * $factor )); 
			xs1=$(( ($xs1-$xe0) * $factor )); ys1=$(( ($ys1-$ye0) * $factor )); 
			xe0=0; ye0=0 ; 
			xe1=$(( $xe_width * $factor )); ye1=$(( $ye_height * $factor )); 
			echo "new params=$xs0,$ys0 ; $xs1,$ys1 ; $xe0,$ye0 ; $xe1,$ye1"
		fi
		[ "$frames" -lt 90 ] && stepsize=1 || stepsize=2 
		stepsize=1
		#echo -n "frame="
		for fr in `seq 1 $stepsize $frames`; do
			[ $fr -lt 100 ] && dj="0$fr" || dj=$fr
			[ $fr -lt 10 ] && dj="00$fr" || dj=$dj
			x0=$(( $xs0 + $(($xe0-$xs0)) * $fr / $frames ))
			y0=$(( $ys0 + $(($ye0-$ys0)) * $fr / $frames ))
			x1=$(( $xs1 + $(($xe1-$xs1)) * $fr / $frames ))
			y1=$(( $ys1 + $(($ye1-$ys1)) * $fr / $frames ))
			x_width=$(( $x1 - $x0 )) ; y_height=$(( $y1 - $y0 ))
#			echo "params=$x0,$y0 ; $x1,$y1 ; $x0,$y0 ; $x1,$y1 ; $x_width x $y_height"
			echo -n "$dj  "
			echo -n "$x_width"x"$y_height"+$x0+$y0" "
			convert -crop "$x_width"x"$y_height"+$x0+$y0 -resize $resolution! -type TrueColorMatte \
			-depth 8 -quality 100 "$outdir/temp_slideshow_image_scaled.ppm" "$outdir"/temp_slideshow_image.ppm		
			cp "$outdir/temp_slideshow_image.ppm" "$outdir/fade_$dj.ppm"
			
			it="`identify -format %wx%h "$outdir/fade_$dj.ppm"`"
			echo $it
			#composite -gravity center -type TrueColorMatte -depth 8 "$outdir/temp_slideshow_image.ppm" \
			#	"$outdir/slideshow_background.ppm" "$outdir/fade_$dj.ppm"
			frame2=$(( $fr + 1 )) # copy another slide
			[ $frame2 -lt 100 ] && dj2="0$frame2" || dj2=$frame2
			[ $frame2 -lt 10 ] && dj2="00$frame2" || dj2=$dj2
			#if [ "$stepsize" -eq 2 ] ; then
				#cp "$outdir/fade_$dj.ppm" "$outdir/fade_$dj2.ppm"
			#fi
			#echo -ne "\b\b\b\b\b\b\b\b\b\b\b"
		done
		echo
		echo "Done."
		cat "$outdir"/fade_*.ppm | ppmtoy4m -v $verbosity -n 0 -S 420mpeg2 -F 30000:1001 -A 10:11 -I p\
			| mpeg2enc -v $verbosity -a 2 -q 4 -s -M 2 -f 8 -o "$outdir/the_slide.mpg"
