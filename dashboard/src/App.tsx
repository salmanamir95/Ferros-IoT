
import { useBrokerPolling } from './hooks/useBrokerPolling';
import { EventFeed } from './components/EventFeed';
import { MetricsPanel } from './components/MetricsPanel';
import { SystemOverview } from './components/SystemOverview';
import { CorrelationExplorer } from './components/CorrelationExplorer';
import { Hexagon } from 'lucide-react';

function App() {
  const { events, isConnected, eventsPerSec } = useBrokerPolling();

  return (
    <div className="app-container">
      <header className="app-header">
        <div style={{ display: 'flex', alignItems: 'center', gap: '0.75rem' }}>
          <div style={{ padding: '0.5rem', background: 'rgba(0, 243, 255, 0.1)', borderRadius: 'var(--radius-md)' }}>
            <Hexagon size={28} color="var(--neon-cyan)" />
          </div>
          <div>
            <h1 style={{ fontSize: '1.5rem', fontWeight: 700, margin: 0, letterSpacing: '-0.02em' }} className="neon-text-cyan">
              FERROS
            </h1>
            <div style={{ fontSize: '0.8rem', color: 'var(--text-muted)', textTransform: 'uppercase', letterSpacing: '0.1em' }}>
              Observability Intelligence
            </div>
          </div>
        </div>
      </header>

      {/* Sidebar / Live Feed */}
      <aside style={{ gridColumn: '1 / 2', gridRow: '2 / -1' }}>
        <EventFeed events={events} />
      </aside>

      {/* Main Content Area */}
      <main className="main-content scroll-hidden" style={{ gridColumn: '2 / -1', gridRow: '2 / -1', paddingRight: '1rem', minHeight: 0 }}>
        <SystemOverview events={events} isConnected={isConnected} eventsPerSec={eventsPerSec} />
        
        <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr', gap: '1.5rem', flex: 1, minHeight: 0, overflow: 'hidden' }}>
          <MetricsPanel events={events} />
          <CorrelationExplorer events={events} />
        </div>
      </main>
    </div>
  );
}

export default App;
