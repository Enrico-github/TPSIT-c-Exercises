const G            = 6.674e-11;
const mL           = 7.348e22;
const dTL          = 3.84e8;
const gL           = 1.66;
const mShip        = 15000;
const SCALE        = 100;
const DT           = 1 / 60;
const THRUST_FORCE = 40000;
const FUEL_MAX     = 100;
const FUEL_RATE    = 0.08;
const MAX_LAND_VY  = 100.0;
const MAX_LAND_VX  = 100.0;

let state, keys, animId, particles, stars, elapsed, paused;

const canvas = document.getElementById('canvas');
const ctx    = canvas.getContext('2d');

/**
 * Ridimensiona il canvas in base alle dimensioni della finestra del browser, NON TOCCARE se non vuoi avere un disastro totale.
 */
function resize() {
  canvas.width  = canvas.offsetWidth;
  canvas.height = canvas.offsetHeight;
}
window.addEventListener('resize', () => {
  resize();
  if (state) state.needRegen = true;
});
resize();

keys = {};
document.addEventListener('keydown', e => {
  if (e.code === 'Escape') {
    togglePause();
    return;
  }
  if (e.code === 'KeyR' && state && !paused) {
    startGame();
    return;
  }
  // Spazio per riavviare dal menu di successo/crash
  if (e.code === 'Space') {
    const successScreen = document.getElementById('ov-success');
    const crashScreen = document.getElementById('ov-crash');
    if (successScreen && successScreen.style.display === 'flex' || 
        crashScreen && crashScreen.style.display === 'flex') {
      startGame();
      return;
    }
  }
  keys[e.code] = true;
  if (['ArrowUp', 'ArrowDown', 'ArrowLeft', 'ArrowRight', 'Space'].includes(e.code)) {
    e.preventDefault();
  }
});
document.addEventListener('keyup', e => { keys[e.code] = false; });

/**
 * Attiva o disattiva la pausa del gioco. Se il gioco non è in pausa, riavvia il loop
 */
function togglePause() {
  if (!state || !state.alive) return;// previene di mettere in pausa quando il gioco non e attivo (previene bug che bloccherebbe l intero gioco)
  paused = !paused;
  showScreen(paused ? 'ov-pause' : null);
  if (!paused) loop();// riavvia il loop se stiamo uscendo dalla pausa, senno rischia di rimanere bloccato
}

/**
 * Genera il terreno lunare con terreno irregolare e 2 piattaforme di atterraggio.
 * Utilizza un algoritmo di suddivisione ricorsiva per creare un terreno naturalistico.
 * @param {number} W - Larghezza del canvas
 * @param {number} H - Altezza del canvas
 * @returns {Object} - Oggetto con array 'points' (punti del terreno) e 'pads' (piattaforme)
 */
function genTerrain(W, H) {
  const groundY = H * 0.72;
  const points  = [];
  const pads    = [];
  const segs    = 64;
  const raw     = new Array(segs + 1);

  raw[0]    = groundY + (Math.random() - 0.5) * 40;
  raw[segs] = groundY + (Math.random() - 0.5) * 40;

  function displace(l, r, rough) {
    if (r - l <= 1) return;
    const m = (l + r) >> 1;
    raw[m] = (raw[l] + raw[r]) / 2 + (Math.random() - 0.5) * rough * (r - l);
    raw[m] = Math.max(H * 0.5, Math.min(H * 0.88, raw[m]));
    displace(l, m, rough * 0.65);
    displace(m, r, rough * 0.65);
  }
  displace(0, segs, 55);

  const xs         = Array.from({ length: segs + 1 }, (_, i) => i * W / segs);
  const numPads    = 2;
  const padIndices = [];
  const candidates = [];

  for (let i = 3; i < segs - 3; i++) {
    if (Math.abs(raw[i + 1] - raw[i - 1]) < 18) candidates.push(i);
  }
  candidates.sort(() => Math.random() - 0.5);

  for (let i = 0; i < candidates.length && padIndices.length < numPads; i++) {
    const idx = candidates[i];
    if (padIndices.every(p => Math.abs(p - idx) > 8)) padIndices.push(idx);
  }

  padIndices.forEach(idx => {
    const flatY = raw[idx];
    for (let k = idx - 2; k <= idx + 2; k++) {
      if (k >= 0 && k <= segs) raw[k] = flatY;
    }
    pads.push({ x: xs[idx - 2], y: flatY, w: xs[idx + 2] - xs[idx - 2] });
  });

  for (let i = 0; i <= segs; i++) points.push({ x: xs[i], y: raw[i] });
  return { points, pads };
}

/**
 * Calcola l'altezza del terreno (coordinata Y) per una data coordinata X.
 * Utilizza l'interpolazione lineare tra i punti del terreno.
 * @param {Array} points - Array di punti del terreno
 * @param {number} x - Coordinata X per cui calcolare l'altezza
 * @returns {number} - Coordinata Y del terreno
 */
function terrainY(points, x) {
  for (let i = 0; i < points.length - 1; i++) {
    if (x >= points[i].x && x <= points[i + 1].x) {
      const t = (x - points[i].x) / (points[i + 1].x - points[i].x);
      return points[i].y + t * (points[i + 1].y - points[i].y);
    }
  }
  return points[points.length - 1].y;
}

/**
 * Genera un array di stelle casuali nel cielo per creare l'atmosfera spaziale.
 * Ogni stella ha posizione, raggio, opacità iniziale e fase di scintillio.
 * @param {number} W - Larghezza del canvas
 * @param {number} H - Altezza del canvas
 * @returns {Array} - Array di oggetti stella con proprietà x, y, r, a, phase
 */
function genStars(W, H) {
  return Array.from({ length: 160 }, () => ({
    x:      Math.random() * W,
    y:      Math.random() * H * 0.65,
    r:      Math.random() * 1.1 + 0.2,
    a:      Math.random() * 0.5 + 0.3,
    phase:  Math.random() * Math.PI * 2
  }));
}

/**
 * Crea particelle per gli effetti visivi: fiamma di spinta e particelle di esplosione/successo.
 * Le particelle hanno velocità, vita, decadimento e colore basati sul tipo.
 * @param {number} x - Coordinata X dove generare le particelle
 * @param {number} y - Coordinata Y dove generare le particelle
 * @param {string} type - Tipo di particelle ('thrust' o 'success'/'crash')
 */
function spawnParticles(x, y, type) {
  const n = type === 'thrust' ? 4 : 28;
  for (let i = 0; i < n; i++) {
    const angle = type === 'thrust'
      ? (Math.PI / 2) + (Math.random() - 0.5) * 0.8
      : Math.random() * Math.PI * 2;
    const speed = type === 'thrust'
      ? Math.random() * 1.5 + 0.5
      : Math.random() * 4 + 1;
    particles.push({
      x, y,
      vx:    Math.cos(angle) * speed,
      vy:    Math.sin(angle) * speed,
      life:  1,
      decay: type === 'thrust' ? 0.07 : 0.034,
      type,
      r:     Math.random() * 2 + 1,
      color: type === 'thrust'
        ? `hsl(${30 + Math.random() * 30},100%,${60 + Math.random() * 30}%)`
        : `hsl(${Math.random() * 40},100%,${50 + Math.random() * 30}%)`
    });
  }
}

/**
 * Mostra o nasconde gli elementi UI sullo schermo (menu, pausa, vittoria, crash).
 * Nascondi tutti gli schermi e mostra solo quello con ID specificato.
 * @param {string} id - ID della schermata da mostrare, null per nascondi tutte
 */
function showScreen(id) {
  ['ov-menu', 'ov-pause', 'ov-success', 'ov-crash'].forEach(s => {
    document.getElementById(s).style.display = 'none';
  });
  if (id) document.getElementById(id).style.display = 'flex';
}

/**
 * Apre il menu principale del gioco, fermando il loop di animazione
 */
function openMenu() {
  cancelAnimationFrame(animId);
  state  = null;
  paused = false;
  showScreen('ov-menu');
}

/**
 * Inizializza un nuovo gioco generando il terreno, le stelle e lo stato della navicella.
 * La navicella parte dall'alto con velocità iniziale minima.
 */
function startGame() {
  cancelAnimationFrame(animId);
  paused = false;
  showScreen(null);

  const W = canvas.width, H = canvas.height;
  const { points, pads } = genTerrain(W, H);
  stars     = genStars(W, H);
  particles = [];
  elapsed   = 0;

  state = {
    x:         W / 2 + (Math.random() - 0.5) * 80,
    y:         55,
    vx:        0,
    vy:        0,
    fuel:      FUEL_MAX,
    points, pads,
    W, H,
    alive:     true,
    thrustOn:  false,
    needRegen: false
  };

  loop();
}

/**
 * Aggiorna la fisica e lo stato del gioco ad ogni frame.
 * Calcola accelerazioni, velocità, posizione, collisioni con il terreno e controlla il carburante.
 * Gestisce il successo dell'atterraggio o il crash.
 */
function update() {
  if (!state.alive) return;

  const s = state;
  elapsed += DT;

  const thrusting = (keys['ArrowUp'] || keys['KeyW']) && s.fuel > 0;
  const goLeft    =  keys['ArrowLeft']  || keys['KeyA'];
  const goRight   =  keys['ArrowRight'] || keys['KeyD'];

  s.thrustOn = thrusting;

  let ax = 0;
  let ay = gL / SCALE;

  if (thrusting) {
    ay -= (THRUST_FORCE / mShip) / SCALE;
    s.fuel = Math.max(0, s.fuel - FUEL_RATE);
  }
  if (goLeft  && s.fuel > 0) { ax -= (THRUST_FORCE * 0.3 / mShip) / SCALE; s.fuel = Math.max(0, s.fuel - FUEL_RATE * 0.4); }
  if (goRight && s.fuel > 0) { ax += (THRUST_FORCE * 0.3 / mShip) / SCALE; s.fuel = Math.max(0, s.fuel - FUEL_RATE * 0.4); }

  s.vx += ax * DT * 60;
  s.vy += ay * DT * 60;
  s.vx *= 0.998;
  s.x  += s.vx * DT * 60;
  s.y  += s.vy * DT * 60;

  if (thrusting && Math.random() < 0.6) spawnParticles(s.x, s.y + 14, 'thrust');

  if (s.x < 15)      { s.x = 15;       s.vx =  Math.abs(s.vx) * 0.3; }
  if (s.x > s.W - 15){ s.x = s.W - 15; s.vx = -Math.abs(s.vx) * 0.3; }
  if (s.y < 0)       { s.y = 0;        s.vy =  Math.abs(s.vy) * 0.3; }

  const ty = terrainY(s.points, s.x);
  if (s.y + 14 >= ty - 1) {
    s.y     = ty - 14;
    s.alive = false;

    const realVY = Math.abs(s.vy) * SCALE;
    const realVX = Math.abs(s.vx) * SCALE;
    const onPad  = s.pads.some(p => s.x >= p.x && s.x <= p.x + p.w && Math.abs(ty - p.y) < 3);
    const Fg     = (G * mL * mShip) / (dTL * dTL);

    if (onPad && realVY <= MAX_LAND_VY && realVX <= MAX_LAND_VX) {
      spawnParticles(s.x - 20, s.y, 'success');
      spawnParticles(s.x,      s.y, 'success');
      spawnParticles(s.x + 20, s.y, 'success');
      showSuccess(realVY, realVX, Fg);
    } else {
      spawnParticles(s.x, s.y, 'crash');
      const reason = !onPad
        ? 'Hai mancato la piattaforma di atterraggio.'
        : realVY > MAX_LAND_VY
          ? `Velocità verticale troppo alta: ${realVY.toFixed(2)} m/s`
          : `Velocità orizzontale troppo alta: ${realVX.toFixed(2)} m/s`;
      showCrash(reason, realVY, realVX, Fg);
    }
  }

  particles.forEach(p => {
    p.x += p.vx;
    p.y += p.vy;
    if (p.type === 'thrust') p.vy += 0.05;
    else { p.vx *= 0.97; p.vy += 0.08; }
    p.life -= p.decay;
  });
  particles = particles.filter(p => p.life > 0);
}

/**
 * Aggiorna il HUD (Head-Up Display) con le informazioni di gioco in tempo reale.
 * Mostra altitudine, velocità verticale/orizzontale, carburante, forza gravitazionale e tempo.
 * Cambia i colori degli indicatori in base alle condizioni (ok, warning, danger).
 */
function updateHUD() {
  const s     = state;

  function setVal(id, val, cls) {
    const el = document.getElementById(id);
    el.textContent = val;
    el.className   = 'hud-val ' + cls;
  }

  setVal('h-vy',   (s.vy * SCALE).toFixed(2),           Math.abs(s.vy * SCALE) > MAX_LAND_VY ? 'danger' : 'ok');
  setVal('h-vx',   (s.vx * SCALE).toFixed(2),           Math.abs(s.vx * SCALE) > MAX_LAND_VX ? 'danger' : 'ok');
  setVal('h-fuel', s.fuel.toFixed(1) + '%',              s.fuel < 20 ? 'danger' : s.fuel < 50 ? 'warn' : 'ok');
  setVal('h-fg',   (G * mL * mShip / (dTL * dTL)).toExponential(2) + ' N', 'ok');
  document.getElementById('h-time').textContent = elapsed.toFixed(1) + ' s';

  const bar = document.getElementById('fuel-bar');
  bar.style.width      = s.fuel.toFixed(1) + '%';
  bar.style.background = s.fuel < 20 ? 'var(--red)' : s.fuel < 50 ? 'var(--amber)' : 'var(--green)';

  document.getElementById('thrust-light').className = s.thrustOn ? 'on' : '';
}

/**
 * Disegna tutti gli elementi grafici della scena sulla canvas.
 * Include il cielo, le stelle, la Terra lontana, il terreno, i crateri, le piattaforme,
 * le particelle, la navicella con tutti i dettagli e la guida di altitudine.
 */
function draw() {
  const s = state;
  const W = s.W, H = s.H;
  const t = performance.now() / 1000;

  ctx.clearRect(0, 0, W, H);

  // Cielo
  const sky = ctx.createLinearGradient(0, 0, 0, H);
  sky.addColorStop(0,   '#000510');
  sky.addColorStop(0.6, '#010820');
  sky.addColorStop(1,   '#0a0505');
  ctx.fillStyle = sky;
  ctx.fillRect(0, 0, W, H);

  // Stelle
  stars.forEach(st => {
    const alpha = st.a + Math.sin(st.phase + t * 1.4) * 0.18;
    ctx.beginPath();
    ctx.arc(st.x, st.y, st.r, 0, Math.PI * 2);
    ctx.fillStyle = `rgba(200,220,255,${alpha})`;
    ctx.fill();
  });

  // Riempimento terreno
  const pts = s.points;
  ctx.beginPath();
  ctx.moveTo(0, H);
  ctx.lineTo(pts[0].x, pts[0].y);
  for (let i = 1; i < pts.length; i++) ctx.lineTo(pts[i].x, pts[i].y);
  ctx.lineTo(W, H);
  ctx.closePath();
  const terrainG = ctx.createLinearGradient(0, H * 0.5, 0, H);
  terrainG.addColorStop(0,   '#2a2010');
  terrainG.addColorStop(0.3, '#1a1408');
  terrainG.addColorStop(1,   '#0d0a04');
  ctx.fillStyle = terrainG;
  ctx.fill();

  // Bordo terreno
  ctx.beginPath();
  ctx.moveTo(pts[0].x, pts[0].y);
  for (let i = 1; i < pts.length; i++) ctx.lineTo(pts[i].x, pts[i].y);
  ctx.strokeStyle = 'rgba(100,80,40,0.5)';
  ctx.lineWidth   = 1.5;
  ctx.stroke();

  // Piattaforme di atterraggio
  s.pads.forEach(pad => {
    ctx.fillStyle = 'rgba(0, 255, 136, 0.88)';
    ctx.font      = '8px monospace';
    ctx.textAlign = 'center';
    ctx.fillText('LZ', pad.x + pad.w / 2, pad.y - 9);
  });

  // Navicella
  ctx.save();
  ctx.translate(s.x, s.y);

  // Fiamma di spinta
  if (s.thrustOn) {
    const flicker = 0.75 + Math.random() * 0.45;
    const fG = ctx.createLinearGradient(0, 10, 0, 10 + 26 * flicker);
    fG.addColorStop(0,   'rgba(255,200,50,0.9)');
    fG.addColorStop(0.5, 'rgba(255,100,20,0.55)');
    fG.addColorStop(1,   'rgba(255,50,0,0)');
    ctx.beginPath();
    ctx.moveTo(-6, 12);
    ctx.bezierCurveTo(-9, 19, -4, 28 * flicker, 0, 32 * flicker);
    ctx.bezierCurveTo(4, 28 * flicker, 9, 19, 6, 12);
    ctx.fillStyle = fG;
    ctx.fill();
  }

  // Propulsori laterali
  if (keys['ArrowLeft']  && state.fuel > 0) drawSideThruster( 12, 0,  1);
  if (keys['ArrowRight'] && state.fuel > 0) drawSideThruster(-12, 0, -1);

  // Gambe
  ctx.strokeStyle = '#8aaa88';
  ctx.lineWidth   = 1.5;
  [-1, 1].forEach(side => {
    ctx.beginPath(); ctx.moveTo(side * 7, 8);  ctx.lineTo(side * 13, 14); ctx.stroke();
    ctx.beginPath(); ctx.moveTo(side * 13, 14); ctx.lineTo(side * 10, 14); ctx.stroke();
  });

  // Corpo
  const bodyG = ctx.createLinearGradient(-9, -10, 9, 10);
  bodyG.addColorStop(0,   '#c8d8c0');
  bodyG.addColorStop(0.4, '#a0b898');
  bodyG.addColorStop(1,   '#607060');
  ctx.beginPath();
  ctx.roundRect(-9, -10, 18, 20, 3);
  ctx.fillStyle   = bodyG;
  ctx.fill();
  ctx.strokeStyle = '#90a890';
  ctx.lineWidth   = 1;
  ctx.stroke();

  // Boccaporto superiore
  ctx.beginPath();
  ctx.arc(0, -10, 5, Math.PI, 0);
  ctx.fillStyle = '#c0d8d0';
  ctx.fill();

  // Finestra
  ctx.beginPath();
  ctx.arc(0, -4, 3, 0, Math.PI * 2);
  ctx.fillStyle = '#001a30';
  ctx.fill();
  ctx.beginPath();
  ctx.arc(-1, -5, 1, 0, Math.PI * 2);
  ctx.fillStyle = 'rgba(100,200,255,0.45)';
  ctx.fill();

  ctx.restore();

  // Barra del carburante (sul lato sinistro)
  const barW = 20, barH = H * 0.4;
  const barX = 14, barY = H / 2 - barH / 2;
  const fuelPercent = s.fuel / FUEL_MAX;
  
  // Riempimento carburante con gradiente di colore in base al livello
  const fillH = barH * fuelPercent;
  const fuelColor = s.fuel < 20 ? '#ff3366' : s.fuel < 50 ? '#ffb300' : '#00ff88';
  ctx.fillStyle = fuelColor;
  ctx.fillRect(barX, barY + barH - fillH, barW, fillH);
  
  // Effetto bagliore nella barra del carburante
  ctx.shadowColor = fuelColor;
  ctx.shadowBlur = 8;
  ctx.fillRect(barX, barY + barH - fillH, barW, fillH);
  ctx.shadowBlur = 0;
  
  // Testo percentuale carburante
  ctx.fillStyle = fuelColor;
  ctx.font = 'bold 11px monospace';
  ctx.textAlign = 'center';
  ctx.fillText((s.fuel).toFixed(0), barX + barW / 2, barY + barH + 20);
}

/**
 * Disegna il propulsore laterale della navicella durante una manovra di rotazione.
 * Crea una fiamma con gradiente che si estende nella direzione specificata.
 * @param {number} x - Coordinata X del propulsore
 * @param {number} y - Coordinata Y del propulsore
 * @param {number} dir - Direzione della fiamma (1 o -1)
 */
function drawSideThruster(x, y, dir) {
  const fG = ctx.createLinearGradient(x, y, x + dir * 13, y);
  fG.addColorStop(0, 'rgba(255,180,40,0.75)');
  fG.addColorStop(1, 'rgba(255,80,0,0)');
  ctx.beginPath();
  ctx.moveTo(x, y - 3);
  ctx.lineTo(x + dir * 13, y);
  ctx.lineTo(x, y + 3);
  ctx.fillStyle = fG;
  ctx.fill();
}

/**
 * Il loop principale del gioco che si esegue continuamente tramite requestAnimationFrame.
 * Rigenera il terreno se la finestra è stata ridimensionata, aggiorna lo stato e disegna la scena.
 */
function loop() {
  if (paused) return;
  if (state.needRegen) {
    const { points, pads } = genTerrain(canvas.width, canvas.height);
    state.points    = points;
    state.pads      = pads;
    state.W         = canvas.width;
    state.H         = canvas.height;
    stars           = genStars(canvas.width, canvas.height);
    state.needRegen = false;
  }
  update();
  draw();
  if (state.alive) updateHUD();
  animId = requestAnimationFrame(loop);
}

/**
 * Mostra la schermata di successo quando la navicella atterra con successo.
 * Visualizza le statistiche finali: velocità, carburante rimasto, tempo di missione e forza gravitazionale.
 * @param {number} vy - Velocità verticale all'atterraggio (m/s)
 * @param {number} vx - Velocità orizzontale all'atterraggio (m/s)
 * @param {number} Fg - Forza gravitazionale calcolata (N)
 */
function showSuccess(vy, vx, Fg) {
  document.getElementById('stats-success').innerHTML =
    `Velocità verticale <span>${vy.toFixed(2)} m/s</span><br>
     Velocità orizzontale <span>${vx.toFixed(2)} m/s</span><br>
     Forza gravitazionale Fg <span>${Fg.toExponential(2)} N</span><br>
     Carburante rimasto <span>${state.fuel.toFixed(1)}%</span><br>
     Tempo missione <span>${elapsed.toFixed(1)} s</span><br>
     (Realisticamente, saresti esploso, ma sarebbe troppo complicato atterrare in una simulazione quindi complimenti!)</span>`;
  setTimeout(() => showScreen('ov-success'), 600);
}

/**
 * Mostra la schermata di crash quando la navicella colpisce il terreno senza successo.
 * Visualizza il motivo del crash e le statistiche al momento dell'impatto.
 * @param {string} reason - Descrizione del motivo del crash
 * @param {number} vy - Velocità verticale all'impatto (m/s)
 * @param {number} vx - Velocità orizzontale all'impatto (m/s)
 * @param {number} Fg - Forza gravitazionale calcolata (N)
 */
function showCrash(reason, vy, vx, Fg) {
  document.getElementById('crash-reason').textContent = reason;
  document.getElementById('stats-crash').innerHTML =
    `Velocità verticale <span>${vy.toFixed(2)} m/s</span><br>
     Velocità orizzontale <span>${vx.toFixed(2)} m/s</span><br>
     Forza gravitazionale Fg <span>${Fg.toExponential(2)} N</span><br>
     Tempo missione <span>${elapsed.toFixed(1)} s</span>`;
  setTimeout(() => showScreen('ov-crash'), 700);
}

// Show menu on load
openMenu();
