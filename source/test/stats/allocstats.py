"""
Do simple analysis of alloc stats.
"""

import csv

GWEN_STATS = "gwen_allocs.csv"
GWK_STATS  = "gwork_allocs.csv"
CURR_STATS = "curr_allocs.csv"
CSS_FILE = "report.css"

class AllocStats:
    def __init__(self, name):
        self.name = name
        self._colnames = []
        self._data = []
        
    def readCsvFile(self, fname):
        with open(fname, 'rb') as csvfile:
            sr = csv.reader(csvfile)
            for i, row in enumerate(sr):
                if i==0:
                    self._colnames = row
                else:
                    self._data.append(row)
        self.process()
        
    def process(self):
        seen, diff = {}, {}
        for d in self._data:
            name = d[0]
            CumNumAllocs,CumAllocSize,CurrNumAllocs,CurrAllocSize = map(int, d[1:])
            if not name in seen:
                seen[name] = (CurrNumAllocs, CurrAllocSize)
            else:
                cal, csz = seen[name]
                nd = (CurrNumAllocs - cal, CurrAllocSize - csz)
                diff[name] = nd
        self._diffs = diff
        
    def dump(self):
        for k in sorted(self._diffs.keys()):
            d = self._diffs[k]
            print k, d[0], d[1]
            
    def report(self, hr):
        hr.el('h2', '%s allocation stats' % self.name)
        hr.el('table', cls='reporttab')
        hr.th(['Name', 'Alloc count', 'Alloc size'])
        for k in sorted(self._diffs.keys()):
            d = self._diffs[k]
            hr.tr([k, npad(d[0],6), XB(d[1])])
        hr.el('/table')
            
            
class AllocDiff:
    def __init__(self, a,b):
        self.a, self.b = a, b
        delta = {}
        da, db = a._diffs, b._diffs
        for ka,va in da.items():
            if ka in db:
                vb = db[ka]
                pctSz = 100.0 * vb[1] / va[1]
                delta[ka] = (vb[0] - va[0], vb[1] - va[1], pctSz)
        self.delta = delta
        
    def dump(self):
        print 'Diff', self.a.name, '->', self.b.name
        for k in sorted(self.delta.keys()):
            d = self.delta[k]
            print k, d[0], d[1], '%.1f%%' % d[2]
            
    def report(self, hr):
        hr.el('h2', 'Stats comparison: %s to %s' % (self.a.name, self.b.name))
        hr.el('table', cls='reporttab')
        hr.th(['Name', 'Count delta', 'Size delta', '% size'])
        for k in sorted(self.delta.keys()):
            d = self.delta[k]
            hr.tr([k, npad(d[0],6), XB(d[1]), '%.1f%%' % d[2]])
        hr.el('/table')


class HtmlReport:
    def __init__(self, css):
        self._h = []
        self.el('html').el('head', self.et('style', css)).el('body')

    def et(self, el, content = None, cls = None):
        h = '<%s class="%s">' % (el, cls) if cls else '<%s>' % el
        if content:
            h += '%s</%s>' % (content, el)
        return h
        
    def el(self, el, content = None, cls = None):
        self._h.append(self.et(el, content, cls))
        return self

    def _trow(self, cols, tel):
        for c in cols:
            self.el(tel, c)
        self.el('/tr')
    def th(self, cols):
        self._trow(cols, 'th')
    def tr(self, cols):
        self._trow(cols, 'td')
    
    def toString(self):
        self.el('/body').el('/html')
        return '\n'.join(self._h)

def toFile(fname, str):
    with open(fname, 'w') as f:
        f.write(str)
        
def readFile(fname):
    with open(fname, 'rb') as f:
        return f.read()
        
def XB(n):
    if abs(n) > 1024:
        return '%s (%.3fKB)' % (npad(n,10), n/1024.0)
    return npad(n,10)
    
def npad(n, s=8):
    fmt = '%%%dd' % s
    return fmt % n
        
                    
def main():
    gwen = AllocStats('GWEN')
    gwen.readCsvFile(GWEN_STATS)

    gwk = AllocStats('Gwork reference')
    gwk.readCsvFile(GWK_STATS)
    
    curr = AllocStats('Gwork current')
    curr.readCsvFile(CURR_STATS)
    
    ggd = AllocDiff(gwen, gwk)
    ggd.dump()
    gcd = AllocDiff(gwk, curr)
    gcd.dump()

    hr = HtmlReport(readFile(CSS_FILE))
    hr.el('h1', 'Gwork Memory Allocation Stats')
    gwen.report(hr)
    curr.report(hr)
    ggd.report(hr)
    gcd.report(hr)
    
    txtRep = hr.toString()
    # print txtRep
    toFile('alloc_report.html', txtRep)

main()
