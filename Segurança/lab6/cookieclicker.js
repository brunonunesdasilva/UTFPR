//Cookie Clicker Bot
// Autor: Bruno Nunes da Silva (RA: 2479800)
// Uso: Cole no console do navegador enquanto joga Cookie Clicker

(function() {
    'use strict';
    
    class CookieBot {
        constructor() {
            this.counter = 0;
            this.intervals = {};
            this.config = {
                autoClick: true,
                goldenCookie: true,
                autoUpgrade: true,
                autoBuilding: true,
                intervals: {
                    click: 25,
                    goldenCookie: 50,
                    upgrade: 6000,
                    building: 10000
                }
            };
            
            this.loadConfig();
            this.init();
        }
        
        loadConfig() {
            const savedConfig = localStorage.getItem('cookieBotConfig');
            if (savedConfig) {
                this.config = JSON.parse(savedConfig);
            }
        }
        
        saveConfig() {
            localStorage.setItem('cookieBotConfig', JSON.stringify(this.config));
        }
        
        init() {
            this.setupBot();
            this.addControls();
        }
        
        setupBot() {
            if (this.config.autoClick) this.startAutoClick();
            if (this.config.goldenCookie) this.startGoldenCookieCheck();
            if (this.config.autoUpgrade) this.startUpgradeCheck();
            if (this.config.autoBuilding) this.startBuildingCheck();
        }
        
        clickBigCookie() {
            try {
                const bigCookie = document.querySelector('#bigCookie');
                if (bigCookie) {
                    bigCookie.click();
                    this.counter++;
                    return true;
                }
            } catch (e) {
                this.log('Erro ao clicar no biscoito', 'error');
            }
            return false;
        }
        
        checkGoldenCookies() {
            try {
                const goldenCookies = document.querySelectorAll('.shimmer');
                goldenCookies.forEach(cookie => {
                    cookie.click();
                    this.log('Biscoito dourado coletado!');
                });
                return goldenCookies.length > 0;
            } catch (e) {
                this.log('Erro ao verificar biscoitos dourados', 'error');
                return false;
            }
        }
        
        buyUpgrades() {
            try {
                let upgrade = document.querySelector('.upgrade.enabled') || 
                              document.querySelector('.crate.upgrade.enabled');
                
                if (upgrade) {
                    upgrade.click();
                    this.log('Upgrade comprado');
                    return true;
                }
            } catch (e) {
                this.log('Erro ao comprar upgrade', 'error');
            }
            return false;
        }
        
        buyBuildings() {
            try {
                const buildings = Array.from(document.querySelectorAll('.product.unlocked.enabled'));
                if (buildings.length > 0) {
                    const building = buildings[buildings.length - 1];
                    building.click();
                    this.log('Construção comprada');
                    return true;
                }
            } catch (e) {
                this.log('Erro ao comprar construção', 'error');
            }
            return false;
        }
        
        startAutoClick() {
            this.stopAction('autoClick');
            this.intervals.autoClick = setInterval(
                () => this.clickBigCookie(), 
                this.config.intervals.click
            );
            this.log('Auto-clique ativado');
        }
        
        startGoldenCookieCheck() {
            this.stopAction('goldenCookie');
            this.intervals.goldenCookie = setInterval(
                () => this.checkGoldenCookies(), 
                this.config.intervals.goldenCookie
            );
            this.log('Busca por biscoitos dourados ativada');
        }
        
        startUpgradeCheck() {
            this.stopAction('upgrade');
            this.intervals.upgrade = setInterval(
                () => this.buyUpgrades(), 
                this.config.intervals.upgrade
            );
            this.log('Compra automática de upgrades ativada');
        }
        
        startBuildingCheck() {
            this.stopAction('building');
            this.intervals.building = setInterval(
                () => this.buyBuildings(), 
                this.config.intervals.building
            );
            this.log('Compra automática de construções ativada');
        }
        
        stopAction(action) {
            if (this.intervals[action]) {
                clearInterval(this.intervals[action]);
                delete this.intervals[action];
            }
        }
        
        stopAll() {
            Object.keys(this.intervals).forEach(action => {
                this.stopAction(action);
            });
            this.log('Todas ações paradas');
        }
        
        toggleFeature(feature) {
            this.config[feature] = !this.config[feature];
            this.saveConfig();
            
            if (this.config[feature]) {
                this['start' + feature.charAt(0).toUpperCase() + feature.slice(1)]();
            } else {
                this.stopAction(feature);
            }
            
            this.updateControls();
        }
        
        log(message, type = 'info') {
            const logElement = document.getElementById('botLog');
            if (logElement) {
                const logEntry = document.createElement('div');
                logEntry.textContent = `[${new Date().toLocaleTimeString()}] ${message}`;
                
                const colors = {
                    info: '#3498db',
                    error: '#e74c3c'
                };
                logEntry.style.color = colors[type] || '#3498db';
                logEntry.style.margin = '2px 0';
                logEntry.style.fontSize = '12px';
                
                logElement.prepend(logEntry);
                
                while (logElement.children.length > 5) {
                    logElement.removeChild(logElement.lastChild);
                }
            }
        }
        
        addControls() {
            const existingControls = document.getElementById('cookieBotControls');
            if (existingControls) existingControls.remove();
            
            const controls = document.createElement('div');
            controls.id = 'cookieBotControls';
            controls.style.cssText = `
                position: fixed;
                bottom: 10px;
                right: 10px;
                z-index: 9999;
                background: white;
                padding: 15px;
                border: 1px solid #ddd;
                border-radius: 8px;
                font-family: Arial, sans-serif;
                box-shadow: 0 2px 10px rgba(0,0,0,0.1);
                width: 250px;
                max-height: 60vh;
                overflow-y: auto;
            `;
            
            const title = document.createElement('h3');
            title.textContent = 'Controle do Bot';
            title.style.margin = '0 0 10px 0';
            title.style.padding = '0';
            title.style.fontSize = '16px';
            title.style.color = '#333';
            controls.appendChild(title);
            
            const features = [
                { 
                    name: 'autoClick', 
                    label: 'Auto-clique',
                    desc: 'Clica automaticamente no biscoito principal' 
                },
                { 
                    name: 'goldenCookie', 
                    label: 'Biscoitos Dourados',
                    desc: 'Coleta biscoitos dourados automaticamente' 
                },
                { 
                    name: 'autoUpgrade', 
                    label: 'Auto-upgrades',
                    desc: 'Compra upgrades quando disponíveis' 
                },
                { 
                    name: 'autoBuilding', 
                    label: 'Auto-construções',
                    desc: 'Compra construções automaticamente' 
                }
            ];
            
            features.forEach(feat => {
                const container = document.createElement('div');
                container.style.margin = '8px 0';
                
                const label = document.createElement('label');
                label.style.display = 'flex';
                label.style.alignItems = 'center';
                label.style.cursor = 'pointer';
                
                const checkbox = document.createElement('input');
                checkbox.type = 'checkbox';
                checkbox.checked = this.config[feat.name];
                checkbox.style.marginRight = '8px';
                checkbox.style.cursor = 'pointer';
                checkbox.onchange = () => this.toggleFeature(feat.name);
                
                const text = document.createElement('span');
                text.textContent = feat.label;
                text.style.fontWeight = 'bold';
                
                label.appendChild(checkbox);
                label.appendChild(text);
                container.appendChild(label);
                
                const desc = document.createElement('div');
                desc.textContent = feat.desc;
                desc.style.fontSize = '11px';
                desc.style.color = '#666';
                desc.style.marginLeft = '22px';
                desc.style.marginTop = '2px';
                container.appendChild(desc);
                
                controls.appendChild(container);
            });
            
            const stopBtn = document.createElement('button');
            stopBtn.textContent = 'Parar Tudo';
            stopBtn.style.cssText = `
                background: #f44336;
                color: white;
                border: none;
                padding: 8px 12px;
                border-radius: 4px;
                margin-top: 10px;
                cursor: pointer;
                width: 100%;
                font-weight: bold;
            `;
            stopBtn.onclick = () => this.stopAll();
            controls.appendChild(stopBtn);
            
            const logTitle = document.createElement('div');
            logTitle.textContent = 'Últimas ações:';
            logTitle.style.margin = '15px 0 5px 0';
            logTitle.style.fontSize = '12px';
            logTitle.style.fontWeight = 'bold';
            controls.appendChild(logTitle);
            
            const logContainer = document.createElement('div');
            logContainer.id = 'botLog';
            logContainer.style.cssText = `
                height: 80px;
                overflow-y: auto;
                font-size: 11px;
                background: #f5f5f5;
                padding: 5px;
                border-radius: 3px;
                border: 1px solid #eee;
            `;
            controls.appendChild(logContainer);
            
            document.body.appendChild(controls);
        }
        
        updateControls() {
            const controls = document.getElementById('cookieBotControls');
            if (controls) {
                ['autoClick', 'goldenCookie', 'autoUpgrade', 'autoBuilding'].forEach(feat => {
                    const checkbox = controls.querySelector(`input[type="checkbox"][onchange*="${feat}"]`);
                    if (checkbox) {
                        checkbox.checked = this.config[feat];
                    }
                });
            }
        }
    }

    // Inicia o bot
    setTimeout(() => {
        new CookieBot();
    }, 2000);
})();