function hideNav (){
    alert("You're a member now!")
}

function showReview(n){
    document.querySelectorAll(".review-con")[n].style.opacity = "100%"
    document.querySelectorAll(".borders")[n].style.filter = "brightness(0.4)" 
}

function dontShowReview(n){
    document.querySelectorAll(".review-con")[n].style.opacity = "0%"
    document.querySelectorAll(".borders")[n].style.filter = "brightness(1)" 
}
var body = document.getElementsByTagName("body")[0]
var header = document.getElementsByTagName("header")[0]
var nav = document.querySelectorAll(".main-nav a")

if(localStorage.getItem('mode') === 'dark'){
    document.querySelector("#sun-icon").style.display = "none"
    document.querySelector("#moon-icon").style.display = "block"
    document.querySelector("#logo").style.display = "none"
    document.querySelector("#logo-dm").style.display = "block"
    body.classList.add("dark-mode-body")
    header.classList.add("dark-mode-header")
    for(var i=0; i<nav.length; i++){
        nav[i].classList.add("dark-mode-nav")
    }
}
function darkMode(){
    document.querySelector("#sun-icon").style.display = "none"
    document.querySelector("#moon-icon").style.display = "block"
    document.querySelector("#logo").style.display = "none"
    document.querySelector("#logo-dm").style.display = "block"
    body.classList.toggle("dark-mode-body")
    header.classList.toggle("dark-mode-header")
    for(var i=0; i<nav.length; i++){
        nav[i].classList.toggle("dark-mode-nav")
    }
    localStorage.setItem('mode', 'dark')
}

function lightMode(){
    document.querySelector("#moon-icon").style.display = "none"
    document.querySelector("#sun-icon").style.display = "block"
    document.querySelector("#logo-dm").style.display = "none"
    document.querySelector("#logo").style.display = "block"
    body.classList.toggle("dark-mode-body")
    header.classList.toggle("dark-mode-header")
    for(var i=0; i<nav.length; i++){
        nav[i].classList.toggle("dark-mode-nav")
    }
    localStorage.setItem('mode', 'light')
}

var number1 = ""
var number2 = ""
var isNumber1Ready =  false
var equalSign = document.getElementById("equal")

function formNumber(n){
    if(!isNumber1Ready){
        number1 += n
        document.getElementById("displayNum").textContent = number1
    }else{
        number2 += n
        document.getElementById("displayNum").textContent = number2
    }
}

function sum(n1, n2){
    return n1+n2
}

function sub(n1, n2){
    return n1-n2
}

function mult(n1, n2){
    return n1*n2
}

function div(n1, n2){
    return n1/n2
}

function mod(n1, n2){
    return n1%n2
}

function cal(n1, n2, op){
    n2 = Number(n2)
    var result = op(n1,n2)
    document.getElementById("displayNum").textContent = result
}

function opSum(){
    number1 = Number(number1)
    isNumber1Ready = true
    equalSign.setAttribute("onclick", "cal(number1, number2, sum)")
}

function opSub(){
    number1 = Number(number1)
    isNumber1Ready = true
    equalSign.setAttribute("onclick", "cal(number1, number2, sub)")
}

function opMult(){
    number1 = Number(number1)
    isNumber1Ready = true
    equalSign.setAttribute("onclick", "cal(number1, number2, mult)")
}

function opDiv(){
    number1 = Number(number1)
    isNumber1Ready = true
    equalSign.setAttribute("onclick", "cal(number1, number2, div)")
}

function opMod(){
    number1 = Number(number1)
    isNumber1Ready = true
    equalSign.setAttribute("onclick", "cal(number1, number2, mod)")
}

function resetCal(){
    document.getElementById("displayNum").textContent = 0
    number1 = ""
    isNumber1Ready = false
    number2 = ""
}

var form = document.getElementById("signup-form")
//CONSTRUCTOR
function CreateUser(name, lastname, email, password){
    this.name = name
    this.lastname = lastname
    this.email = email
    this.password = password
}

function newUserSignup(){
    var userName = form.elements[0].value
    var userLastName = form.elements[1].value
    var userEmail = form.elements[2].value
    var userPsswrd = form.elements[3].value
    var newUser = new CreateUser(userName, userLastName, userEmail, userPsswrd)
    console.log(newUser)
    alert("New User: "+userName+" "+userLastName)
}