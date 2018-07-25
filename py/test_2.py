import truetypetracer as ttt

#wr = ttt.NGC_Writer()
wr = ttt.SEG_Writer()

# wr.scale = 1
wr.arc = False
wr.conic = False
wr.cubic = False
s3 = ttt.ttt("LT",wr)
print s3
s = wr.get_segments()
print s
e = wr.extents
print "number of segments ",len(s)
print e
print "extents max x",e.maxx
print "extents min x",e.minx
print "extents max y",e.maxy
print "extents min y",e.miny
