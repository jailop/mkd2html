window.onload = init;
scaled = 0;
current = 0;

function init()
{
	window.onkeydown = changeFrame;
	divs = document.getElementsByTagName('div');
	els = divs.length;
	for (i in divs) {
		if (i > 0) divs[i].setAttribute('class', 'hidden');
	}
	document.getElementById('nav:home').onclick = moveHome;
	document.getElementById('nav:prev').onclick = movePrev;
	document.getElementById('nav:next').onclick = moveNext;
	document.getElementById('nav:last').onclick = moveLast;
	document.getElementById('nav:zoom').onclick = scaleFrame;
}

function changeFrame(ev)
{
	switch (ev.keyCode)
	{
		case 33:
			movePrev(); break;
		case 34:
			movePrev(); break;
		case 35:
			moveLast(); break;
		case 36:
			moveHome(); break;
		case 37:
			movePrev(); break;
		case 38:
			movePrev(); break;
		case 39:
			moveNext(); break;
		case 40:
			moveNext(); break;
		case 80: /* Key: p */
			scaleFrame(); break;
		case 90: /* Key: z */
			scaleFrame(); break;
	}
}

function moveHome()
{
	if (current > 0)
	{
		divs[current].setAttribute('class', 'hidden');
		current = 0;
		divs[current].setAttribute('class', 'frame');
	}
}

function movePrev()
{
	if (current > 0)
	{
		divs[current].setAttribute('class', 'hidden');
		current--;
		divs[current].setAttribute('class', 'frame');
	}
}

function moveNext()
{
	if (current + 1 < els)
	{
		divs[current].setAttribute('class', 'hidden');
		current++;
		divs[current].setAttribute('class', 'frame');
	}
}

function moveLast()
{
	if (current + 1 != els)
	{
		divs[current].setAttribute('class', 'hidden');
		current = els - 1;
		divs[current].setAttribute('class', 'frame');
	}
}

function scaleFrame()
{
	if (scaled == 0)
	{
		var th = window.innerHeight;
		var tw = window.innerWidth;
		for (i = 0; i < els; i++)
		{
			if (i != current)
				divs[i].setAttribute('class', 'frame');
			var dh = divs[i].clientHeight;
			var dw = divs[i].clientWidth;
			var sy = th / dh * 0.96;
			var sx = tw / dw * 0.98;
			var ss = sx > sy ? sy : sx;
			ss = dh > th ? sx : ss;
			sty = 'transform: scale('+ ss + ',' + ss + ')';
			sty = '-webkit-' + sty + ';' + sty;
			divs[i].style = sty; 
			if (i != current)
				divs[i].setAttribute('class', 'hidden');
		}
		scaled = 1;
	}
	else
	{
		for (i = 0; i < els; i++)
		{
			if (i != current)
				divs[i].setAttribute('class', 'frame');
			sty = 'transform: scale(1, 1)';
			sty = '-webkit-' + sty + ';' + sty;
			divs[i].style = sty;
			if (i != current)
				divs[i].setAttribute('class', 'hidden');
		}
		scaled = 0;
	}
}

