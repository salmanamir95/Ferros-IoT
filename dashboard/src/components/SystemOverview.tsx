import { useMemo, useEffect, useState } from 'react';
import type { FerrosInsight } from '../types/ferros';
import { Server, Activity, Users, Zap } from 'lucide-react';

interface Props {
  events: FerrosInsight[];
  isConnected: boolean;
  eventsPerSec: number;
}

export const SystemOverview: React.FC<Props> = ({ events, isConnected, eventsPerSec }) => {
  const stats = useMemo(() => {
    const uniqueProcs = new Set(events.map(e => e.identity.comm));
    
    // Most noisy process
    const procCounts: Record<string, number> = {};
    events.forEach(e => {
      procCounts[e.identity.comm] = (procCounts[e.identity.comm] || 0) + 1;
    });
    
    let noisyProc = "None";
    let maxCount = 0;
    Object.entries(procCounts).forEach(([proc, count]) => {
      if (count > maxCount) {
        maxCount = count;
        noisyProc = proc;
      }
    });

    return {
      activeProcesses: uniqueProcs.size,
      noisyProc
    };
  }, [events]);

  return (
    <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(200px, 1fr))', gap: '1.5rem' }}>
      
      <div className="glass-card" style={{ padding: '1.5rem', display: 'flex', alignItems: 'center', gap: '1rem' }}>
        <div style={{ padding: '1rem', borderRadius: '50%', background: 'rgba(0, 243, 255, 0.1)', color: 'var(--neon-cyan)' }}>
          <Server size={24} />
        </div>
        <div>
          <div style={{ color: 'var(--text-muted)', fontSize: '0.9rem' }}>Broker Status</div>
          <div style={{ fontSize: '1.5rem', fontWeight: 700, color: isConnected ? 'var(--neon-green)' : 'var(--neon-red)' }}>
            {isConnected ? 'Connected' : 'Offline'}
          </div>
        </div>
      </div>

      <div className="glass-card" style={{ padding: '1.5rem', display: 'flex', alignItems: 'center', gap: '1rem' }}>
        <div style={{ padding: '1rem', borderRadius: '50%', background: 'rgba(176, 38, 255, 0.1)', color: 'var(--neon-purple)' }}>
          <Activity size={24} />
        </div>
        <div>
          <div style={{ color: 'var(--text-muted)', fontSize: '0.9rem' }}>Events / Sec</div>
          <div style={{ fontSize: '1.5rem', fontWeight: 700 }}>{eventsPerSec}</div>
        </div>
      </div>

      <div className="glass-card" style={{ padding: '1.5rem', display: 'flex', alignItems: 'center', gap: '1rem' }}>
        <div style={{ padding: '1rem', borderRadius: '50%', background: 'rgba(255, 153, 0, 0.1)', color: 'var(--neon-orange)' }}>
          <Users size={24} />
        </div>
        <div>
          <div style={{ color: 'var(--text-muted)', fontSize: '0.9rem' }}>Observed Procs</div>
          <div style={{ fontSize: '1.5rem', fontWeight: 700 }}>{stats.activeProcesses}</div>
        </div>
      </div>

      <div className="glass-card" style={{ padding: '1.5rem', display: 'flex', alignItems: 'center', gap: '1rem' }}>
        <div style={{ padding: '1rem', borderRadius: '50%', background: 'rgba(255, 42, 42, 0.1)', color: 'var(--neon-red)' }}>
          <Zap size={24} />
        </div>
        <div>
          <div style={{ color: 'var(--text-muted)', fontSize: '0.9rem' }}>Noisiest Process</div>
          <div style={{ fontSize: '1.25rem', fontWeight: 700, whiteSpace: 'nowrap', overflow: 'hidden', textOverflow: 'ellipsis', maxWidth: '120px' }}>
            {stats.noisyProc}
          </div>
        </div>
      </div>

    </div>
  );
};
