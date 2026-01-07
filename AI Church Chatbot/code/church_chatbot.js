
const DEFAUT_LOGO = `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="width: 60%; height: 60%; color: white;"><path d="M21 11.5a8.38 8.38 0 0 1-.9 3.8 8.5 8.5 0 0 1-7.6 4.7 8.38 8.38 0 0 1-3.8-.9L3 21l1.9-5.7a8.38 8.38 0 0 1-.9-3.8 8.5 8.5 0 0 1 4.7-7.6 8.38 8.38 0 0 1 3.8-.9h.5a8.48 8.48 0 0 1 8 8v.5z"/><path d="M12 7v6"/><path d="M9 10h6"/></svg>`;

class ChurchChatbot extends HTMLElement {
    constructor() {
        super();
        this.attachShadow({ mode: 'open' });
        this.chatHistory = [];
        this.apiUrl = this.getAttribute('api-url') || 'http://localhost:8004/chat';
        this.chatbotTitle = this.getAttribute('title') || 'Church Assistant';
        this.churchId = this.getAttribute('church-id') || null; // Support Multi-Tenancy
        this.greeting = this.getAttribute('greeting') || "Hi there! I'm your digital greeter! I am here to answer any questions you may have about our church and help you get connected. How can I help you?";
        this.logoContent = this.getAttribute('logo-svg') || DEFAUT_LOGO;
    }

    connectedCallback() {
        this.injectDependencies();
        this.render();
        this.setupEventListeners();
    }

    injectDependencies() {
        if (!window.marked) {
            const script = document.createElement('script');
            script.src = 'https://cdn.jsdelivr.net/npm/marked/marked.min.js';
            document.head.appendChild(script);
        }
    }

    render() {
        const style = `
        <style>
            :host {
                /* Inherit font or use default */
                font-family: 'Inter', system-ui, -apple-system, sans-serif;
                z-index: 99999;
                position: fixed;
                bottom: 0;
                right: 0;

                /* MODERN CLEAN EMERALD THEME */
                /* A professional, fresh church website look. No yellow. */
                --primary-gradient: linear-gradient(145deg, #065f46 0%, #047857 100%); /* Emerald 800 -> 700 */
                --primary-solid: #065f46;
                --accent-color: #059669; /* Emerald 600 */
                --accent-bg: #ecfdf5; /* Emerald 50 */
                
                --bg-chat: #ffffff; /* Pure White - Clean */
                --text-main: #1f2937; /* Gray 800 - Sharp & Modern */
                --text-muted: #6b7280; /* Gray 500 */
                --border-light: #e5e7eb; /* Gray 200 */
                
                --shadow-float: 0 10px 40px -10px rgba(6, 95, 70, 0.4); /* Deep emerald shadow */
                --shadow-soft: 0 4px 6px -1px rgba(0, 0, 0, 0.05), 0 2px 4px -1px rgba(0, 0, 0, 0.03); /* Subtle neutral shadow */

                --radius-xl: 20px; /* Slightly tighter, more professional */
                --radius-sm: 8px;
            }

            * { box-sizing: border-box; }

            /* 1. Toggle Button - Dark Green & Modern */
            #chat-toggle {
                position: fixed;
                bottom: 30px;
                right: 30px;
                width: 72px;
                height: 72px;
                background: var(--primary-gradient);
                color: white;
                border-radius: 50%;
                box-shadow: var(--shadow-float);
                display: flex;
                justify-content: center;
                align-items: center;
                cursor: pointer;
                z-index: 10000;
                overflow: hidden;
                transition: transform 0.2s cubic-bezier(0.34, 1.56, 0.64, 1);
            }

            #chat-toggle:hover {
                transform: scale(1.1);
            }

            /* 2. Chat Window */
            #chat-window {
                position: fixed;
                bottom: 120px; /* Moved up to clear the toggle button (72px + 30px + padding) */
                right: 30px;
                width: 380px;
                height: 650px;
                max-height: calc(100vh - 140px); /* Adjusted max-height to keep it on screen */
                background: var(--bg-chat);
                border-radius: var(--radius-xl);
                box-shadow: var(--shadow-soft);
                display: none;
                flex-direction: column;
                overflow: hidden;
                z-index: 9999;
                opacity: 0;
                transform: translateY(20px);
                transition: opacity 0.3s cubic-bezier(0.2, 0.8, 0.2, 1), transform 0.3s cubic-bezier(0.2, 0.8, 0.2, 1);
                border: 1px solid rgba(255, 255, 255, 0.8);
            }

            #chat-window.open {
                opacity: 1;
                transform: translateY(0);
            }

            /* Header */
            #chat-header {
                background-color: var(--primary-solid);
                background: var(--primary-gradient);
                color: white;
                padding: 20px;
                display: flex;
                align-items: center;
                justify-content: space-between;
                flex-shrink: 0;
                box-shadow: 0 4px 12px rgba(0,0,0,0.1);
            }

            .header-info {
                display: flex;
                align-items: center;
                gap: 12px;
            }

            .header-avatar {
                width: 40px;
                height: 40px;
                justify-content: center;
                display: flex;
                align-items: center;
                font-size: 20px;
                overflow: hidden;
                border-radius: 50%;
                background: rgba(255,255,255,0.2);
                border: none;
            }

            .header-text h3 {
                margin: 0;
                font-size: 17px;
                font-weight: 700;
                letter-spacing: 0.3px;
            }

            .header-text span {
                font-size: 13px;
                opacity: 0.95;
            }

            #chat-close-btn {
                cursor: pointer;
                opacity: 0.8;
                transition: opacity 0.2s;
            }

            #chat-close-btn:hover {
                opacity: 1;
            }

            /* Chat History */
            #chat-history {
                flex: 1;
                padding: 24px;
                overflow-y: auto;
                overflow-x: hidden; /* Prevent horizontal scroll on mobile */
                background: var(--bg-chat);
                display: flex;
                flex-direction: column;
                gap: 20px;
                scrollbar-width: thin;
                scrollbar-color: rgba(63, 98, 18, 0.2) transparent;
            }

            /* Messages */
            .msg-container {
                display: flex;
                align-items: flex-end;
                gap: 12px;
                opacity: 0;
                animation: fadeIn 0.3s forwards;
            }

            @keyframes fadeIn {
                to { opacity: 1; }
            }

            .user-container {
                flex-direction: row-reverse;
            }

            .bot-avatar-sml {
                width: 40px;
                height: 40px;
                background: var(--primary-solid); /* Use Primary Color */
                color: white;
                border: none;
                border-radius: 50%;
                display: flex;
                align-items: center;
                justify-content: center;
                font-size: 16px;
                flex-shrink: 0;
                overflow: hidden;
                box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
            }
            
            .bot-avatar-sml svg {
                width: 60%;
                height: 60%;
            }

            .msg {
                max-width: 82%; /* Slightly reduced to prevent edge cases */
                padding: 14px 18px;
                font-size: 14.5px;
                line-height: 1.55;
                position: relative;
                word-wrap: break-word;
                overflow-wrap: break-word; /* Modern wrapping */
            }

            .bot-msg {
                background: #f3f4f6; /* Gray 100 - Clean & Neutral */
                border: none; /* Cleaner look without border */
                border-radius: 20px 20px 20px 4px;
                color: var(--text-main);
                box-shadow: none; /* Flat design is more modern */
            }

            .user-msg {
                background: var(--primary-gradient);
                color: white;
                border-radius: 20px 20px 4px 20px;
                box-shadow: 0 4px 6px rgba(0,0,0,0.1); /* Subtle shadow */
                font-weight: 400; /* Lighter font weight looks more premium */
            }

            .bot-msg p { margin: 0 0 10px 0; }
            .bot-msg p:last-child { margin-bottom: 0; }
            .bot-msg ul { margin: 8px 0; padding-left: 20px; }
            .bot-msg li { margin-bottom: 6px; }
            .bot-msg a { color: var(--accent-color); text-decoration: none; font-weight: 600; }
            .bot-msg a:hover { text-decoration: underline; color: var(--primary-solid); }
            .bot-msg strong { font-weight: 600; color: #111827; /* Gray 900 */ }

            /* Source Footer */
            .sources-footer {
                margin-top: 10px;
                padding-top: 10px;
                border-top: 1px dashed #d1d5db;
                font-size: 11px;
                color: var(--text-muted);
                display: flex;
                align-items: center;
                gap: 5px;
                overflow: hidden; /* Ensure container doesn't spill */
            }
            .sources-footer a {
                white-space: nowrap;
                overflow: hidden;
                text-overflow: ellipsis;
                max-width: 100%;
                display: block; /* Required for ellipsis to work */
                color: var(--text-muted);
                text-decoration: none;
            }
            .sources-footer a:hover {
                text-decoration: underline;
                color: var(--primary-solid);
            }

            /* Interactive Link Cards */
            .interactive-card {
                background: #ffffff;
                border: 1px solid var(--border-light);
                border-radius: 12px;
                padding: 14px;
                margin: 10px 0;
                cursor: pointer;
                transition: all 0.2s ease;
                list-style: none;
                margin-left: -20px;
                display: block;
                box-shadow: 0 1px 2px rgba(0,0,0,0.05);
            }
            .interactive-card:hover {
                border-color: var(--accent-color);
                background: #ffffff;
                box-shadow: 0 10px 15px -3px rgba(0, 0, 0, 0.1);
                transform: translateY(-2px);
            }
            .interactive-card strong {
                display: block;
                color: var(--primary-solid);
                margin-bottom: 4px;
            }

            /* Chips */
            .chips-container {
                display: flex;
                flex-wrap: wrap;
                gap: 8px;
                margin-left: 48px;
                margin-top: 10px;
            }
            .chip {
                background: #ffffff;
                border: 1px solid var(--border-light);
                padding: 8px 16px;
                border-radius: 20px;
                font-size: 13px;
                font-weight: 500;
                color: var(--text-main);
                cursor: pointer;
                transition: all 0.2s ease;
                box-shadow: 0 1px 2px rgba(0,0,0,0.05);
            }
            .chip:hover {
                border-color: var(--accent-color);
                color: var(--primary-solid);
                background: white;
                transform: translateY(-1px);
                box-shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.1);
            }
            .chip svg {
                margin-right: 6px;
                vertical-align: text-bottom;
                width: 14px; /* Slightly smaller icons */
                height: 14px;
            }

            /* Input Area */
            #chat-input-area {
                padding: 16px 20px;
                background: #ffffff;
                border-top: 1px solid var(--border-light);
                display: flex;
                gap: 12px;
                align-items: center;
                z-index: 10;
            }
            #user-input {
                flex: 1;
                padding: 14px 20px;
                border: 1px solid var(--border-light);
                background: #f9fafb; /* Gray 50 */
                border-radius: 24px;
                font-size: 15px;
                outline: none;
                transition: all 0.2s;
                font-family: inherit;
                color: var(--text-main);
            }
            #user-input:focus {
                background: #ffffff;
                border-color: var(--accent-color);
                box-shadow: 0 0 0 3px rgba(16, 185, 129, 0.15); /* Soft Emerald glow */
            }
            #send-btn {
                width: 44px;
                height: 44px;
                border-radius: 50%;
                background: var(--primary-gradient);
                color: white;
                border: none;
                cursor: pointer;
                display: flex;
                align-items: center;
                justify-content: center;
                transition: transform 0.2s ease;
                box-shadow: 0 4px 6px rgba(0,0,0,0.1);
            }
            #send-btn:hover { transform: scale(1.05); }
            #send-btn svg { fill: currentColor; width: 22px; height: 22px; margin-left: 2px; }

            /* Typing Dots */
            #loading-dots { display: flex; gap: 4px; padding: 6px 0; }
            .dot {
                width: 6px; height: 6px; background: var(--text-muted); border-radius: 50%;
                animation: bounce 1.4s infinite ease-in-out both;
            }
            .dot:nth-child(1) { animation-delay: -0.32s; }
            .dot:nth-child(2) { animation-delay: -0.16s; }
            @keyframes bounce {
                0%, 80%, 100% { transform: scale(0); }
                40% { transform: scale(1.0); }
            }
        </style>`;

        const html = `
        <!-- TOGGLE BUTTON -->
        <div id="chat-toggle" role="button" aria-label="Open Chat">
            ${this.logoContent}
        </div>

        <!-- CHAT WINDOW -->
        <div id="chat-window">
            <!-- Header -->
            <div id="chat-header">
                <div class="header-info">
                    <div class="header-avatar">
                        ${this.logoContent}
                    </div>
                    <div class="header-text">
                        <h3>${this.chatbotTitle}</h3>
                        <span>Digital Greeter</span>
                    </div>
                </div>
                <div id="chat-close-btn" role="button" aria-label="Close Chat">
                    <svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round">
                        <line x1="18" y1="6" x2="6" y2="18"></line>
                        <line x1="6" y1="6" x2="18" y2="18"></line>
                    </svg>
                </div>
            </div>

            <!-- History -->
            <div id="chat-history">
                <!-- Greeting -->
                <div class="msg-container bot-container">
                    <div class="bot-avatar-sml">
                        ${this.logoContent}
                    </div>
                    <div class="msg bot-msg">
                        <p>${this.greeting}</p>
                        <p>Select one of the options below or type out your own message and I'd be happy to help you!</p>
                    </div>
                </div>

                <!-- Chips -->
                <div class="chips-container" id="initial-chips">
                    <button class="chip" data-msg="What are your service times?">
                        <svg width="15" height="15" viewBox="0 0 24 24" fill="#e0e7ff" stroke="#6366f1" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"></circle><polyline points="12 6 12 12 16 14"></polyline></svg> Service Times
                    </button>
                    <button class="chip" data-msg="Where are you located?">
                        <svg width="15" height="15" viewBox="0 0 24 24" fill="#d1fae5" stroke="#10b981" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M21 10c0 7-9 13-9 13s-9-6-9-13a9 9 0 0 1 18 0z"></path><circle cx="12" cy="10" r="3"></circle></svg> Location
                    </button>
                    <button class="chip" data-msg="How can I give?">
                        <svg width="15" height="15" viewBox="0 0 24 24" fill="#ffe4e6" stroke="#f43f5e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M20.84 4.61a5.5 5.5 0 0 0-7.78 0L12 5.67l-1.06-1.06a5.5 5.5 0 0 0-7.78 7.78l1.06 1.06L12 21.23l7.78-7.78 1.06-1.06a5.5 5.5 0 0 0 0-7.78z"></path></svg> Giving
                    </button>
                    <button class="chip" data-msg="I am new here!">
                        <svg width="15" height="15" viewBox="0 0 24 24" fill="#fef3c7" stroke="#f59e0b" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2"></path><circle cx="12" cy="7" r="4"></circle></svg> I'm New
                    </button>
                     <button class="chip" data-msg="What events are coming up?">
                        <svg width="15" height="15" viewBox="0 0 24 24" fill="#f3e8ff" stroke="#9333ea" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="3" y="4" width="18" height="18" rx="2" ry="2"></rect><line x1="16" y1="2" x2="16" y2="6"></line><line x1="8" y1="2" x2="8" y2="6"></line><line x1="3" y1="10" x2="21" y2="10"></line></svg> Events
                    </button>
                     <button class="chip" data-msg="I have a prayer request">
                        <svg width="15" height="15" viewBox="0 0 24 24" fill="#ecfeff" stroke="#06b6d4" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M21 11.5a8.38 8.38 0 0 1-.9 3.8 8.5 8.5 0 0 1-7.6 4.7 8.38 8.38 0 0 1-3.8-.9L3 21l1.9-5.7a8.38 8.38 0 0 1-.9-3.8 8.5 8.5 0 0 1 4.7-7.6 8.38 8.38 0 0 1 3.8-.9h.5a8.48 8.48 0 0 1 8 8v.5z"></path></svg> Prayer
                    </button>
                </div>
            </div>

            <!-- Input -->
            <div id="chat-input-area">
                <input type="text" id="user-input" placeholder="Ask a question..." autocomplete="off">
                <button id="send-btn" aria-label="Send Message">
                    <svg viewBox="0 0 24 24"><path d="M2.01 21L23 12 2.01 3 2 10l15 2-15 2z"></path></svg>
                </button>
            </div>
        </div>
        `;

        this.shadowRoot.innerHTML = style + html;
    }

    setupEventListeners() {
        // Bind UI elements
        this.chatWindow = this.shadowRoot.getElementById('chat-window');
        this.toggleBtn = this.shadowRoot.getElementById('chat-toggle');
        this.closeBtn = this.shadowRoot.getElementById('chat-close-btn');
        this.inputField = this.shadowRoot.getElementById('user-input');
        this.sendBtn = this.shadowRoot.getElementById('send-btn');
        this.chipsContainer = this.shadowRoot.getElementById('initial-chips');
        this.historyDiv = this.shadowRoot.getElementById('chat-history');

        // Toggle logic
        this.toggleBtn.onclick = () => this.toggleChat();
        this.closeBtn.onclick = () => this.toggleChat();

        // Send logic
        this.sendBtn.onclick = () => this.sendMessage();
        this.inputField.onkeypress = (e) => {
            if (e.key === 'Enter') this.sendMessage();
        };

        // Chips logic
        const chips = this.shadowRoot.querySelectorAll('.chip');
        chips.forEach(chip => {
            chip.onclick = () => {
                const msg = chip.getAttribute('data-msg');
                this.sendPreset(msg);
            };
        });
    }

    toggleChat() {
        const isHidden = getComputedStyle(this.chatWindow).display === 'none';
        if (isHidden) {
            this.chatWindow.style.display = 'flex';
            setTimeout(() => {
                this.chatWindow.classList.add('open');
                this.scrollToBottom();
                this.inputField.focus();
            }, 10);
        } else {
            this.chatWindow.classList.remove('open');
            setTimeout(() => { this.chatWindow.style.display = 'none'; }, 200);
        }
    }

    sendPreset(text) {
        this.inputField.value = text;
        this.sendMessage();
        // User requested chips remain visible and clickable multiple times
    }

    scrollToBottom() {
        this.historyDiv.scrollTo({ top: this.historyDiv.scrollHeight, behavior: 'smooth' });
    }

    async sendMessage() {
        const text = this.inputField.value.trim();
        if (!text) return;

        // 1. User Msg
        this.historyDiv.insertAdjacentHTML('beforeend', `
            <div class="msg-container user-container">
                <div class="msg user-msg">${text.replace(/</g, "&lt;")}</div>
            </div>`);
        this.chatHistory.push({ role: 'user', content: text });
        this.inputField.value = "";
        this.scrollToBottom();

        // 2. Loading
        const loadingId = "loading-" + Date.now();
        this.historyDiv.insertAdjacentHTML('beforeend', `
            <div class="msg-container bot-container" id="${loadingId}">
                <div class="bot-avatar-sml">
                    ${this.logoContent}
                </div>
                <div class="msg bot-msg" style="min-width: 60px;">
                        <div id="loading-dots"><div class="dot"></div><div class="dot"></div><div class="dot"></div></div>
                </div>
            </div>`);
        this.scrollToBottom();

        try {
            const response = await fetch(this.apiUrl, {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    message: text,
                    history: this.chatHistory,
                    use_full_context: false,
                    church_id: this.churchId
                })
            });

            if (!response.ok) throw new Error("API Error");
            const data = await response.json();
            const rawResponse = data.response;
            const sources = data.sources || []; // Array of URLs

            // Parse Markdown
            let formattedResponse = rawResponse;
            if (window.marked) {
                formattedResponse = window.marked.parse(rawResponse);
            }

            // Remove Loading
            const loadingElem = this.shadowRoot.getElementById(loadingId);
            if (loadingElem) loadingElem.remove();

            // 3. Bot Msg
            let sourcesHtml = "";
            if (sources.length > 0) {
                sourcesHtml = `<div class="sources-footer">
                    <span>Source:</span>
                    ${sources.slice(0, 1).map(s => `<a href="${s}" target="_blank" title="${s}">${new URL(s).hostname + new URL(s).pathname}</a>`).join(", ")}
                </div>`;
            }

            this.historyDiv.insertAdjacentHTML('beforeend', `
                <div class="msg-container bot-container">
                    <div class="bot-avatar-sml">
                        ${this.logoContent}
                    </div>
                    <div class="msg bot-msg">
                        ${formattedResponse}
                        ${sourcesHtml}
                    </div>
                </div>`);
            
            this.chatHistory.push({ role: 'assistant', content: rawResponse });
            this.scrollToBottom();

        } catch (error) {
            console.error(error);
            const loadingElem = this.shadowRoot.getElementById(loadingId);
            if (loadingElem) loadingElem.remove();

            this.historyDiv.insertAdjacentHTML('beforeend', `
                <div class="msg-container bot-container">
                    <div class="bot-avatar-sml" style="background: #ef4444;">!</div>
                    <div class="msg bot-msg">
                        I'm sorry, I'm having trouble connecting to the server right now.
                    </div>
                </div>`);
            this.scrollToBottom();
        }
    }
}

customElements.define('church-chatbot', ChurchChatbot);
