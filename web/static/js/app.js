// Guitar Pedal - Frontend JavaScript

// State management
const state = {
    selectedFile: null,
    sessionId: null,
    presets: {}
};

// DOM elements
const elements = {
    uploadArea: document.getElementById('uploadArea'),
    fileInput: document.getElementById('fileInput'),
    fileInfo: document.getElementById('fileInfo'),
    fileName: document.getElementById('fileName'),
    removeFile: document.getElementById('removeFile'),
    
    presetSelect: document.getElementById('presetSelect'),
    
    pot1: document.getElementById('pot1'),
    pot2: document.getElementById('pot2'),
    pot3: document.getElementById('pot3'),
    pot4: document.getElementById('pot4'),
    
    knob1: document.getElementById('knob1'),
    knob2: document.getElementById('knob2'),
    knob3: document.getElementById('knob3'),
    knob4: document.getElementById('knob4'),
    
    value1: document.getElementById('value1'),
    value2: document.getElementById('value2'),
    value3: document.getElementById('value3'),
    value4: document.getElementById('value4'),
    
    processBtn: document.getElementById('processBtn'),
    resultsSection: document.getElementById('resultsSection'),
    
    originalAudio: document.getElementById('originalAudio'),
    processedAudio: document.getElementById('processedAudio'),
    
    downloadBtn: document.getElementById('downloadBtn'),
    newProcessBtn: document.getElementById('newProcessBtn'),
    
    loadingOverlay: document.getElementById('loadingOverlay'),
    errorMessage: document.getElementById('errorMessage'),
    errorText: document.getElementById('errorText'),
    closeError: document.getElementById('closeError')
};

// Initialize app
function init() {
    setupFileUpload();
    setupKnobs();
    setupPresets();
    setupButtons();
    loadPresets();
}

// File Upload Setup
function setupFileUpload() {
    // Click to upload
    elements.uploadArea.addEventListener('click', () => {
        elements.fileInput.click();
    });
    
    // File selected via input
    elements.fileInput.addEventListener('change', handleFileSelect);
    
    // Drag and drop
    elements.uploadArea.addEventListener('dragover', (e) => {
        e.preventDefault();
        elements.uploadArea.classList.add('drag-over');
    });
    
    elements.uploadArea.addEventListener('dragleave', () => {
        elements.uploadArea.classList.remove('drag-over');
    });
    
    elements.uploadArea.addEventListener('drop', (e) => {
        e.preventDefault();
        elements.uploadArea.classList.remove('drag-over');
        
        const files = e.dataTransfer.files;
        if (files.length > 0) {
            handleFile(files[0]);
        }
    });
    
    // Remove file button
    elements.removeFile.addEventListener('click', (e) => {
        e.stopPropagation();
        resetFileUpload();
    });
}

function handleFileSelect(e) {
    const files = e.target.files;
    if (files.length > 0) {
        handleFile(files[0]);
    }
}

function handleFile(file) {
    // Validate file type
    const validTypes = ['audio/mpeg', 'audio/mp3', 'audio/wav', 'audio/ogg', 'audio/m4a', 'audio/x-m4a'];
    const validExtensions = ['.mp3', '.wav', '.ogg', '.m4a'];
    
    const fileExtension = file.name.substring(file.name.lastIndexOf('.')).toLowerCase();
    
    if (!validExtensions.includes(fileExtension) && !validTypes.includes(file.type)) {
        showError('Invalid file type. Please upload MP3, WAV, OGG, or M4A files.');
        return;
    }
    
    state.selectedFile = file;
    elements.fileName.textContent = file.name;
    elements.uploadArea.style.display = 'none';
    elements.fileInfo.style.display = 'flex';
    elements.processBtn.disabled = false;
    elements.resultsSection.style.display = 'none';
}

function resetFileUpload() {
    state.selectedFile = null;
    elements.fileInput.value = '';
    elements.uploadArea.style.display = 'block';
    elements.fileInfo.style.display = 'none';
    elements.processBtn.disabled = true;
    elements.resultsSection.style.display = 'none';
}

// Knob Controls Setup
function setupKnobs() {
    const knobs = [
        { slider: elements.pot1, knob: elements.knob1, value: elements.value1, format: formatDelay },
        { slider: elements.pot2, knob: elements.knob2, value: elements.value2, format: formatGeneric },
        { slider: elements.pot3, knob: elements.knob3, value: elements.value3, format: formatLFO },
        { slider: elements.pot4, knob: elements.knob4, value: elements.value4, format: formatFeedback }
    ];
    
    knobs.forEach((knob, index) => {
        // Update on slider change
        knob.slider.addEventListener('input', () => {
            updateKnob(knob);
            // Reset preset selection to "Custom"
            elements.presetSelect.value = '';
            
            // If we have an active session, show "Reprocess" button
            if (state.sessionId) {
                elements.processBtn.textContent = '🔄 Reprocess with New Settings';
                elements.processBtn.style.background = 'linear-gradient(135deg, #f59e0b, #d97706)';
            }
        });
        
        // Initialize
        updateKnob(knob);
    });
}

function updateKnob(knob) {
    const value = parseFloat(knob.slider.value);
    const rotation = (value / 100) * 270 - 135; // -135° to +135°
    
    const indicator = knob.knob.querySelector('.knob-indicator');
    indicator.style.transform = `translateX(-50%) rotate(${rotation}deg)`;
    
    knob.value.textContent = knob.format(value);
}

// Format functions for different parameters
function formatDelay(value) {
    return Math.round(value * 10) + 'ms';
}

function formatGeneric(value) {
    return (value / 100).toFixed(2);
}

function formatLFO(value) {
    return (value * 0.04).toFixed(2) + 'ms';
}

function formatFeedback(value) {
    return Math.round(value) + '%';
}

// Presets Setup
function setupPresets() {
    elements.presetSelect.addEventListener('change', (e) => {
        const presetName = e.target.value;
        if (presetName && state.presets[presetName]) {
            applyPreset(state.presets[presetName]);
        }
    });
}

async function loadPresets() {
    try {
        const response = await fetch('/presets');
        const presets = await response.json();
        state.presets = presets;
    } catch (error) {
        console.error('Failed to load presets:', error);
    }
}

function applyPreset(preset) {
    elements.pot1.value = preset.pot1 * 100;
    elements.pot2.value = preset.pot2 * 100;
    elements.pot3.value = preset.pot3 * 100;
    elements.pot4.value = preset.pot4 * 100;
    
    // Update all knobs
    updateKnob({ slider: elements.pot1, knob: elements.knob1, value: elements.value1, format: formatDelay });
    updateKnob({ slider: elements.pot2, knob: elements.knob2, value: elements.value2, format: formatGeneric });
    updateKnob({ slider: elements.pot3, knob: elements.knob3, value: elements.value3, format: formatLFO });
    updateKnob({ slider: elements.pot4, knob: elements.knob4, value: elements.value4, format: formatFeedback });
    
    // If we have an active session, show "Reprocess" button
    if (state.sessionId) {
        elements.processBtn.textContent = '🔄 Reprocess with Preset';
        elements.processBtn.style.background = 'linear-gradient(135deg, #f59e0b, #d97706)';
    }
}

// Button Setup
function setupButtons() {
    elements.processBtn.addEventListener('click', processAudio);
    elements.downloadBtn.addEventListener('click', downloadAudio);
    elements.newProcessBtn.addEventListener('click', resetForNewProcess);
    elements.closeError.addEventListener('click', hideError);
}

// Process Audio (New: Upload once, process multiple times)
async function processAudio() {
    if (!state.selectedFile && !state.sessionId) {
        showError('Please select a file first');
        return;
    }
    
    // Show loading
    elements.loadingOverlay.style.display = 'flex';
    
    try {
        // Step 1: Upload file if not already uploaded
        if (!state.sessionId) {
            const formData = new FormData();
            formData.append('file', state.selectedFile);
            
            const uploadResponse = await fetch('/upload', {
                method: 'POST',
                body: formData
            });
            
            const uploadResult = await uploadResponse.json();
            
            if (!uploadResponse.ok) {
                throw new Error(uploadResult.error || 'Upload failed');
            }
            
            // Store session ID for reprocessing
            state.sessionId = uploadResult.session_id;
            
            // Set up original audio player (only once)
            elements.originalAudio.src = `/play/${state.sessionId}/original`;
        }
        
        // Step 2: Process with current parameters (can be done multiple times!)
        const processResponse = await fetch(`/process/${state.sessionId}`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                effect: 'echo',
                pot1: (elements.pot1.value / 100).toFixed(3),
                pot2: (elements.pot2.value / 100).toFixed(3),
                pot3: (elements.pot3.value / 100).toFixed(3),
                pot4: (elements.pot4.value / 100).toFixed(3)
            })
        });
        
        const processResult = await processResponse.json();
        
        if (!processResponse.ok) {
            throw new Error(processResult.error || 'Processing failed');
        }
        
        // Update processed audio player with new result
        // Add timestamp to force reload
        elements.processedAudio.src = `/play/${state.sessionId}/processed?t=${Date.now()}`;
        
        // Show results
        elements.resultsSection.style.display = 'block';
        elements.resultsSection.scrollIntoView({ behavior: 'smooth' });
        
        // Update button text to show success
        elements.processBtn.textContent = '✅ Processed! Adjust knobs to reprocess';
        elements.processBtn.style.background = 'linear-gradient(135deg, #10b981, #059669)';
        
        // Auto-play the processed audio so user can hear the changes
        setTimeout(() => {
            elements.processedAudio.play().catch(() => {
                // Ignore if autoplay is blocked by browser
            });
        }, 300);
        
    } catch (error) {
        showError(error.message);
    } finally {
        elements.loadingOverlay.style.display = 'none';
    }
}

// Download Audio
function downloadAudio() {
    if (!state.sessionId) {
        showError('No processed audio available');
        return;
    }
    
    window.location.href = `/download/${state.sessionId}`;
}

// Reset for New Process
function resetForNewProcess() {
    // Clean up old session
    if (state.sessionId) {
        fetch(`/cleanup/${state.sessionId}`, { method: 'POST' })
            .catch(err => console.error('Cleanup failed:', err));
    }
    
    resetFileUpload();
    state.sessionId = null;
    
    // Reset button text
    elements.processBtn.textContent = 'Process Audio';
}

// Error Handling
function showError(message) {
    elements.errorText.textContent = message;
    elements.errorMessage.style.display = 'flex';
    
    // Auto-hide after 5 seconds
    setTimeout(hideError, 5000);
}

function hideError() {
    elements.errorMessage.style.display = 'none';
}

// Initialize when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
} else {
    init();
}

