  function doWYSIWYG(command, condition, arg){
    //editorpanelname.focus();
    if(!condition) condition=false;
    var test = document.execCommand(command, condition, arg);
    test.className="Manipulated";
  }
  function doWYSIWYG2(command, condition, arg){
   editorpanelname.focus();
   addPTag(editorpanelname, command);
  }
  function getselectedtext(){
    var txt = '';
    if (window.getSelection){
        txt = window.getSelection();
    }else if (document.getSelection){
        txt = document.getSelection();
    }else if (document.selection){
        txt = document.selection.createRange().text;
    }else
	  return;
    return txt;
  }
  function getTextAreaSelectedText(fromid){
    var txta = document.getElementById(fromid);  
    return (txta.value).substring(txta.selectionStart,txta.selectionEnd);
  }
  var issource =false;
  function tosource(){
  	if (!issource){
  		document.getElementById("Source").value = editorpanelname.innerHTML;
  		issource = true;
  	}
  	//switch to other vieuw
    setclass("Editorpanel",'hidden Apanel');
  	setclass("Sourcepanel",'Apanel');
  }
  
  function totexteditor(){
  	if (issource){
  		editorpanelname.innerHTML = document.getElementById("Source").value;
  		issource = false;
  	}  	
 	//switch to other vieuw
	setclass("Editorpanel",'Apanel p100');
  	setclass("Sourcepanel",'hidden Apanel');
  }
  function senddata(Field){
	tosource();
    /*if (navigator.userAgent.indexOf("Firefox")!=-1){
  		//firefox workaround not works
        alert("firefox useless can not send data");
        document.forms["editor"].submit();
        
  	}else{*/
  	    document.getElementById("editor").submit();
  	//}
    
  }
  function doselect(){
  		if(window.getSelection) {
			s.removeAllRanges();
			s.addRange(rng);
		} else {
			rng.select();
		}
  }
  function doresize(){
    var Y = document.getElementById('height').value;
    var X = document.getElementById('width').value;
    if(issize(X,Y)) { 
		alert("One or more of the dimensions entered cannot be supported. Setting defaults.");
		X="98%"; Y="350";
	}
	replacevalue("width",X);
	replacevalue("height",Y);
	editorpanelname.width=X;
	editorpanelname.height=Y;
  }
  //todo delete _moz_dirty if mozila
  function isbrowserok(){
  		if (navigator.userAgent.indexOf("Firefox")!=-1){
  			return true;
  		}else if(window.navigator.appName == "Microsoft Internet Explorer"){//you must use a better browser
			var IEversion = window.navigator.appVersion;
			alert(window.navigator.appName + "you must use a better browser for this function");
			return (IEversion.substring(IEversion.indexOf("MSIE") + 5, IEversion.indexOf("MSIE") + 8) >= 5.5)
		}else if(window.navigator.appName == "Netscape"){//&&(parseInt(appVersion)==4
		  	return true;	
		}else{//geen ondersteunde browser
			return false;
		}
   }
  	function issize(X,Y){
		var xlen = X.length;
		var xsub = X.substring(0,xlen-1);
		//-- take last char and lie %--&&-- max100 min50
		if((X.substring(xlen-1,xlen))=="%"&&(xsub>100||xsub<50)||X>screen.width||X<200||isNaN(xsub-1)){
			 return false;
		}
		var ylen = Y.length;
		if((Y.substring(ylen-1,ylen))=="%"||Y<200||isNaN(Y.substring(0,ylen-1))) {
			return false;
		}
		return true;		
	}
	function replacevalue(id,value){
			  document.getElementById(id).value=value;
	}
	function setclass(id,value){
			  document.getElementById(id).className=value;
	}