import React, { useState, useMemo } from 'react';
import { FerrosInsight } from '../types/ferros';
import { InsightCard } from './InsightCard';
import { Network, Search } from 'lucide-react';

interface Props {
  events: FerrosInsight[];
}

export const CorrelationExplorer: React.FC<Props> = ({ events }) => {
  const [searchTerm, setSearchTerm] = useState('');

  const filteredEvents = useMemo(() => {
    if (!searchTerm) return [];
    const term = searchTerm.toLowerCase();
    
    return events.filter(e => 
      e.correlation_id.toLowerCase().includes(term) || 
      e.identity.comm.toLowerCase().includes(term)
    );
  }, [events, searchTerm]);

  return (
    <div className="glass-panel" style={{ padding: '1.5rem', display: 'flex', flexDirection: 'column', gap: '1rem', height: '100%' }}>
      <h2 style={{ fontSize: '1.25rem', fontWeight: 600, display: 'flex', alignItems: 'center', gap: '0.5rem' }}>
        <Network size={20} color="var(--neon-purple)" />
        Correlation Explorer
      </h2>

      <div style={{ position: 'relative' }}>
        <input 
          type="text" 
          placeholder="Filter by Correlation ID or Process Name..."
          value={searchTerm}
          onChange={e => setSearchTerm(e.target.value)}
          style={{
            width: '100%',
            padding: '0.75rem 1rem 0.75rem 2.5rem',
            background: 'rgba(0,0,0,0.2)',
            border: '1px solid var(--border-light)',
            borderRadius: 'var(--radius-md)',
            color: 'var(--text-main)',
            outline: 'none'
          }}
        />
        <Search size={18} color="var(--text-muted)" style={{ position: 'absolute', left: '0.75rem', top: '50%', transform: 'translateY(-50%)' }} />
      </div>

      <div className="scroll-hidden" style={{ flex: 1, overflowY: 'auto', display: 'flex', flexDirection: 'column', gap: '1rem', marginTop: '1rem' }}>
        {searchTerm ? (
          filteredEvents.length > 0 ? (
            filteredEvents.map(event => (
              <div key={`${event.correlation_id}_${event.trace.timestamp_ns}`} style={{ position: 'relative' }}>
                <div style={{ position: 'absolute', left: '-1rem', top: '50%', width: '1rem', height: '2px', background: 'var(--border-purple)' }} />
                <InsightCard insight={event} />
              </div>
            ))
          ) : (
            <div style={{ color: 'var(--text-muted)', textAlign: 'center', marginTop: '2rem' }}>No matching events found.</div>
          )
        ) : (
          <div style={{ color: 'var(--text-muted)', textAlign: 'center', marginTop: '2rem' }}>
            Enter a Correlation ID or process name to explore trace chains.
          </div>
        )}
      </div>
    </div>
  );
};
