import { useState, useEffect, useRef } from 'react';
import type { FerrosInsight } from '../types/ferros';

export function useBrokerPolling() {
  const [events, setEvents] = useState<FerrosInsight[]>([]);
  const [isConnected, setIsConnected] = useState(false);
  
  const knownIds = useRef<Set<string>>(new Set());

  useEffect(() => {
    const fetchInsights = async () => {
      try {
        const res = await fetch('/api/insights');
        if (!res.ok) throw new Error('API Error');
        
        const data: FerrosInsight[] = await res.json();
        
        setEvents(prev => {
          const newEvents = data.filter(e => {
            // Deduplicate by correlation_id AND timestamp since some ids might not be globally unique without timestamp
            const uniqueKey = `${e.correlation_id}_${e.trace.timestamp_ns}`;
            if (!knownIds.current.has(uniqueKey)) {
              knownIds.current.add(uniqueKey);
              return true;
            }
            return false;
          });

          if (newEvents.length === 0) return prev;

          // Merge and sort chronologically, keep last 200 to prevent memory leaks
          const merged = [...prev, ...newEvents]
            .sort((a, b) => a.trace.timestamp_ns - b.trace.timestamp_ns)
            .slice(-200);

          // Prune knownIds to avoid massive set
          if (knownIds.current.size > 1000) {
            knownIds.current = new Set(merged.map(e => `${e.correlation_id}_${e.trace.timestamp_ns}`));
          }

          return merged;
        });
        
        setIsConnected(true);
      } catch (err) {
        console.error('Failed to fetch broker output:', err);
        setIsConnected(false);
      }
    };

    // Initial fetch
    fetchInsights();
    
    // Poll every 1s
    const intervalId = setInterval(fetchInsights, 1000);
    return () => clearInterval(intervalId);
  }, []);

  return { events, isConnected };
}
