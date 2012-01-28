import ttt

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
print e
print e.maxx
print e.minx
print e.maxy
print e.miny
