import React from 'react';
import { FerrosInsight } from '../types/ferros';
import { Cpu, Activity, Clock, Hash } from 'lucide-react';

interface Props {
  insight: FerrosInsight;
}

export const InsightCard: React.FC<Props> = ({ insight }) => {
  const formatTime = (ns: number) => {
    // Basic conversion to ms, or formatting as a string
    const d = new Date(ns / 1000000);
    return `${d.toLocaleTimeString()}.${String(d.getMilliseconds()).padStart(3, '0')}`;
  };

  const getBadgeClass = (type: string) => {
    if (type.includes('Pressure') || type.includes('Hotspot') || type.includes('Fragmentation')) return 'badge-red';
    if (type.includes('Entropy') || type.includes('Fairness')) return 'badge-purple';
    return 'badge-cyan';
  };

  return (
    <div className="glass-card" style={{ padding: '1rem', display: 'flex', flexDirection: 'column', gap: '0.75rem' }}>
      <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'flex-start' }}>
        <div>
          <h3 style={{ fontSize: '1.1rem', fontWeight: 600, color: 'var(--text-main)', marginBottom: '0.25rem' }}>
            {insight.identity.comm}
          </h3>
          <span className={`badge ${getBadgeClass(insight.type)}`}>
            {insight.type}
          </span>
        </div>
        <div style={{ display: 'flex', alignItems: 'center', gap: '4px', color: 'var(--text-muted)', fontSize: '0.85rem' }}>
          <Clock size={14} />
          {formatTime(insight.trace.timestamp_ns)}
        </div>
      </div>

      <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr', gap: '0.5rem', marginTop: '0.5rem' }}>
        <div style={{ display: 'flex', alignItems: 'center', gap: '0.5rem', color: 'var(--text-muted)' }}>
          <Hash size={16} color="var(--neon-cyan)" />
          <span style={{ fontSize: '0.9rem' }}>
            PID {insight.identity.pid} <span style={{ opacity: 0.5 }}>/ {insight.identity.tgid}</span>
          </span>
        </div>
        <div style={{ display: 'flex', alignItems: 'center', gap: '0.5rem', color: 'var(--text-muted)' }}>
          <Cpu size={16} color="var(--neon-purple)" />
          <span style={{ fontSize: '0.9rem' }}>Core {insight.trace.cpu}</span>
        </div>
        <div style={{ display: 'flex', alignItems: 'center', gap: '0.5rem', color: 'var(--text-muted)', gridColumn: '1 / -1' }}>
          <Activity size={16} color="var(--neon-green)" />
          <span style={{ fontSize: '0.9rem' }}>Event: {insight.trace.event_type}</span>
        </div>
      </div>
    </div>
  );
};
