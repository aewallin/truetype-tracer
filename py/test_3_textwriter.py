import truetypetracer as ttt
import ConfigParser


def ttt_geometry(text_string="TEST", text_scale=1, subdiv=10):
    """ get geometry from truetypetracer
    """
    wr = ttt.SEG_Writer()
    wr.scale = text_scale
    wr.arc = False
    wr.conic = False
    wr.cubic = False
    
    wr.conic_biarc_subdivision = 10  # this has no effect?
    wr.conic_line_subdivision = subdiv  # this increasesn nr of points to 366
    wr.cubic_biarc_subdivision = 10  # no effect?
    wr.cubic_line_subdivision = 10  # no effect?
    #wr.setFont(2)
    
    s3 = ttt.ttt(text_string,wr)
    print s3
    segments = wr.get_segments()
    # format is 
    # (x, y, r, cw(bool), cx, cy)
    #e = wr.extents
    print "number of loops ",len(segments)
    return segments
    
    
def vertex_idx_generator():
    """ unique integer index for each vertex
    """
    _start = 0
    idx = _start
    while True:
        yield idx
        idx = idx+1

def format_segments(segments):
    """ format segments from ttt into a better format
    """


    n_loop=0
    idx_gen = vertex_idx_generator()
    
    vertices = {}
    line_segments = []
        
    for loop in segments:
        first_pt = loop[0]
        last_pt = loop[-1]
        assert first_pt == last_pt
        loop.pop() # remove last point, since it is the same as the first.
        

        
        #previous_idx = idx_gen.next()
        #vertices[ previous_idx ] = loop[-1]
        #current_idx = idx_gen.next()
        #vertices[ current_idx ] = loop[0]
        
        #line_segments.append( (previous_idx, current_idx) )
        idxs=[]
        for pt in loop:
            current_idx = idx_gen.next()
            vertices[ current_idx ] = pt
            idxs.append(current_idx)
            
        for n in range(len(idxs)):
            
            line_segments.append( ( idxs[n-1], idxs[n]) )

            #previous_idx = current_idx
            #print idx_gen.next()
        print "loop ", n_loop, "ok"

        #break
        n_loop=n_loop+1
    return vertices, line_segments, n_loop
    
def config_writer(fname, text_string, num_loops, scale, subdiv, vertices, line_segments):
    config = ConfigParser.ConfigParser()
    cfgfile = open(fname,'w')
    #print "vertices"
    config.add_section('Info')
    config.set('Info','program', str('TrueTypeTracer'))
    config.set('Info','program_version', str(ttt.version()))
    config.set('Info','url', str('https://github.com/aewallin/truetype-tracer'))
    config.set('Info','Text', str(text_string))
    config.set('Info','num_loops', str(num_loops))
    config.set('Info','scale', str(scale))
    config.set('Info','subdiv', str(subdiv))
    config.set('Info','num_pointsites', str(len(vertices)))
    config.set('Info','num_linesites', str(len(line_segments)))
    
    config.add_section('PointSites')
    for p in vertices:
        print p, vertices[p]
        config.set('PointSites',str(p), str( (vertices[p][0], vertices[p][1]) ))
    #print "line segments"
    config.add_section('LineSites')
    n_line=0
    for p in line_segments:
        config.set('LineSites',str(n_line), str( (p[0], p[1]) ))
        print p, vertices[p[0]],"-",vertices[p[1]]
        n_line=n_line+1
    config.add_section('End')
    config.write(cfgfile)
    cfgfile.close()

if __name__=="__main__":
    txt="TEST"
    scale=1
    subdiv = 100
    segs = ttt_geometry(txt, scale, subdiv)
    verts, lines, n_loops = format_segments(segs)
    fname="a.ini"
    config_writer(fname, txt, n_loops, scale, subdiv, verts, lines)

#print e
#print "extents max x",e.maxx
#print "extents min x",e.minx
#print "extents max y",e.maxy
#print "extents min y",e.miny
