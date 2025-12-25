// assets/app.js - simple helpers for demo navigation & storage

// assets/app.js
function openSosPage(role){
  // open in new tab so the user can continue during countdown
  const win = window.open('sos.html?role=' + encodeURIComponent(role), '_blank');
  if(!win) alert('Popup blocked. Please allow popups for this site to use SOS.');
}


// Simple navigation helper (simulate server redirect)
function go(page, params = {}) {
  // params -> URL query
  const qs = new URLSearchParams(params).toString();
  window.location = page + (qs ? '?' + qs : '');
}


/* localStorage demo helpers */
function getStore(key){ try { return JSON.parse(localStorage.getItem(key)||'null'); } catch(e){ return null; } }
function setStore(key, val){ localStorage.setItem(key, JSON.stringify(val)); }

/* ensure initial demo data */
if(!getStore('ss_routes')){
  setStore('ss_routes', [
    {id:'R1', name:'gwailor → indore', from:'gwailor', to:'indore', price: 20, type:'Bus'},
    {id:'R2', name:'gwailor → Datiya', from:'gwailor', to:'Datiya', price: 15, type:'Bus'},
    {id:'R3', name:'gwailor → agra', from:'gwailor', to:'agra', price: 120, type:'Bus'}
  ]);
}
if(!getStore('ss_users')) setStore('ss_users', {}); // uid -> user object
if(!getStore('ss_bookings')) setStore('ss_bookings', []);
