# crf-shimo

<p><b>Description:A optimized version of crf++0.58 with a little faster and add some postprocess for chinese segment. </b></p>

<p><b>Install: </b></p>
<p>	&nbsp;&nbsp;&nbsp;&nbsp;a)&nbsp;/configure [--prefix=/xxx/xxx] </p>
<p>	&nbsp;&nbsp;&nbsp;&nbsp;b)&nbsp;make </p>
<p>	&nbsp;&nbsp;&nbsp;&nbsp;c)&nbsp;sudo make install </p>

<p><b>Run: </b></p>
<p><b>&nbsp;&nbsp;&nbsp;&nbsp;traning: </b></p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;see example/chseg/train.sh </p>
<p><b>&nbsp;&nbsp;&nbsp;&nbsp;test: </b></p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;see example/chseg/test.sh </p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;You can download a pre-trained chinese model(utf8,trained by 10073281 sentences and 56397344 features) from http://pan.baidu.com/s/1eRbYelW</p>

<p><b>ChangeLog: </b></p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;@2016.02.18 toshimo </p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;1. avoid re-allocate memory frequently: it may be a little fast(about 5%+ with 50,000,000 features).</p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;@2016.03.03 toshimo </p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;1. add a chinese segment model(utf8)</p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;2. add some postprocess paramters for chinese segment</p>

<p><b>@author toshimo@126.com</b></p>

